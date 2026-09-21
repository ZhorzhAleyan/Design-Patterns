#include "AddExpenseDialog.h"

#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

namespace {
// ---- Named constants (no magic numbers) -----------------------------------
const QString    kUiDateFormat  = QStringLiteral("yyyy-MM-dd");
constexpr double kMaxAmount     = 1'000'000.0;  // upper bound for the amount spin box
constexpr int    kAmountDecimals = 2;
}  // namespace

AddExpenseDialog::AddExpenseDialog(const QStringList& existingCategories, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("Add expense"));

    // --- Date: defaults to today, with a calendar popup. ---
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat(kUiDateFormat);

    // --- Category: editable so the user can reuse one or type a new one. ---
    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setEditable(true);
    m_categoryCombo->addItems(existingCategories);
    m_categoryCombo->setCurrentText(QString());  // start blank -> forces a choice

    // --- Amount: a spin box makes negative or non-numeric input impossible. ---
    m_amountSpin = new QDoubleSpinBox(this);
    m_amountSpin->setRange(0.0, kMaxAmount);
    m_amountSpin->setDecimals(kAmountDecimals);

    // --- Note: free text, optional. ---
    m_noteEdit = new QLineEdit(this);
    m_noteEdit->setPlaceholderText(tr("Optional description"));

    auto* form = new QFormLayout;
    form->addRow(tr("Date:"), m_dateEdit);
    form->addRow(tr("Category:"), m_categoryCombo);
    form->addRow(tr("Amount:"), m_amountSpin);
    form->addRow(tr("Note:"), m_noteEdit);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // Validate before accepting; Cancel just closes with QDialog::reject().
    connect(buttons, &QDialogButtonBox::accepted, this, &AddExpenseDialog::validateAndAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

void AddExpenseDialog::validateAndAccept() {
    const QString category = m_categoryCombo->currentText().trimmed();
    if (category.isEmpty()) {
        QMessageBox::warning(this, tr("Missing category"),
                             tr("Please choose or type a category."));
        return;  // keep the dialog open so the user can fix it
    }
    if (m_amountSpin->value() <= 0.0) {
        QMessageBox::warning(this, tr("Invalid amount"),
                             tr("The amount must be greater than zero."));
        return;
    }

    // All good: build the result. id stays 0 -> the database assigns the real id.
    m_result.id       = 0;
    m_result.date     = m_dateEdit->date();
    m_result.category = category;
    m_result.amount   = m_amountSpin->value();
    m_result.note     = m_noteEdit->text().trimmed();

    accept();
}
