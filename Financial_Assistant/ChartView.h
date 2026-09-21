#ifndef FINANCIAL_ASSISTANT_CHARTVIEW_H
#define FINANCIAL_ASSISTANT_CHARTVIEW_H

#include <vector>

#include <QWidget>

#include "ChartDataBuilder.h"
#include "Expense.h"

class QComboBox;
class QDateEdit;
class QLabel;
class QPushButton;
class QChartView;  // Qt 6 keeps the chart classes in the global namespace

/**
 * @brief UI layer: shows expenses as a line, bar or pie chart with filters.
 *
 * The widget is a pure consumer of data: callers hand it a
 * std::vector<Expense> via setExpenses(), and it draws whatever it is given.
 * It contains no SQL and does not know about the database. When the user asks
 * to import a CSV it merely emits importCsvRequested() and lets the owner
 * (which holds the repository) do the actual work, then call setExpenses()
 * again. This keeps the data/logic/UI layers cleanly separated.
 */
class ChartView : public QWidget {
    Q_OBJECT

public:
    explicit ChartView(QWidget* parent = nullptr);

    /// Replace the dataset and refresh every control and the chart.
    void setExpenses(const std::vector<Expense>& expenses);

signals:
    /// Emitted after the user picks a CSV file to import (absolute path).
    void importCsvRequested(const QString& filePath);

    /// Emitted after the user fills in the manual "Add expense" form.
    void addExpenseRequested(const Expense& expense);

private slots:
    void onFilterChanged();       ///< Any filter/chart-type change -> redraw.
    void onImportButtonClicked(); ///< Open a file dialog, then emit the signal.
    void onAddButtonClicked();    ///< Open the Add-expense form, then emit the signal.

private:
    // ---- Chart building ------------------------------------------------
    void rebuildChart();
    void showLineChart(const std::vector<Expense>& data);
    void showBarChart(const std::vector<Expense>& data);
    void showPieChart(const std::vector<Expense>& data);
    void showMessage(const QString& text);  ///< Replace the chart with a centred message.

    // ---- Helpers -------------------------------------------------------
    void buildControls();
    void refreshCategoryCombo();
    ExpenseFilter currentFilter() const;
    static QString truncateLabel(const QString& text);

    // ---- Data ----------------------------------------------------------
    std::vector<Expense> m_expenses;  ///< The full, unfiltered dataset.

    // ---- Widgets (all parented, so Qt frees them automatically) --------
    QComboBox*   m_chartTypeCombo = nullptr;
    QDateEdit*   m_fromDateEdit   = nullptr;
    QDateEdit*   m_toDateEdit     = nullptr;
    QComboBox*   m_categoryCombo  = nullptr;
    QPushButton* m_addButton      = nullptr;
    QPushButton* m_importButton   = nullptr;
    QChartView*  m_chartView      = nullptr;
    QLabel*      m_statusLabel    = nullptr;
};

#endif // FINANCIAL_ASSISTANT_CHARTVIEW_H
