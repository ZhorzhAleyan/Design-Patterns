#ifndef FINANCIAL_ASSISTANT_ADDEXPENSEDIALOG_H
#define FINANCIAL_ASSISTANT_ADDEXPENSEDIALOG_H

#include <QDialog>
#include <QStringList>

#include "Expense.h"

class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QLineEdit;

/**
 * @brief Modal form for entering a single expense by hand (the third data
 *        source from the spec: SQLite, CSV import, and manual input).
 *
 * The dialog is pure UI: it only gathers and validates user input and exposes
 * the result via expense(). It never touches the database — the caller takes
 * the returned Expense and hands it to the repository. This keeps the manual
 * input path on the same layered flow as CSV import.
 */
class AddExpenseDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @param existingCategories Known categories, offered in an editable combo
     *        box so the user can pick an existing one or type a brand new one.
     */
    explicit AddExpenseDialog(const QStringList& existingCategories,
                              QWidget* parent = nullptr);

    /// The expense built from the form (valid only after the dialog is accepted).
    Expense expense() const { return m_result; }

private slots:
    /// Validate the fields; accept only if they are sound, otherwise warn.
    void validateAndAccept();

private:
    QDateEdit*      m_dateEdit     = nullptr;
    QComboBox*      m_categoryCombo = nullptr;
    QDoubleSpinBox* m_amountSpin   = nullptr;
    QLineEdit*      m_noteEdit     = nullptr;

    Expense m_result;  ///< Filled in on successful validation.
};

#endif // FINANCIAL_ASSISTANT_ADDEXPENSEDIALOG_H
