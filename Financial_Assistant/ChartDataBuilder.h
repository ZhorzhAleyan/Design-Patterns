#ifndef FINANCIAL_ASSISTANT_CHARTDATABUILDER_H
#define FINANCIAL_ASSISTANT_CHARTDATABUILDER_H

#include <vector>

#include <QDate>
#include <QPair>
#include <QString>
#include <QVector>

#include "Expense.h"

/**
 * @brief Time grouping used by the line chart.
 */
enum class Period {
    Daily,   ///< Group totals by exact day.
    Monthly  ///< Group totals by calendar month (represented as the 1st of the month).
};

/**
 * @brief The filter applied to the raw expense list before aggregation.
 *
 * An invalid QDate means "no bound", and an empty (or "All") category means
 * "any category". Keeping the filter as a small value type makes the logic
 * layer trivial to unit-test without any UI.
 */
struct ExpenseFilter {
    QDate   from;        ///< Inclusive lower bound; invalid => no lower bound.
    QDate   to;          ///< Inclusive upper bound; invalid => no upper bound.
    QString category;    ///< Category to keep; empty or "All" => every category.
};

/**
 * @brief Pure logic layer: turns a list of expenses into aggregated chart data.
 *
 * The class is entirely stateless (only static methods) so it can be reused and
 * tested in isolation. It never touches the database or the UI: it receives a
 * std::vector<Expense> and returns aggregated series.
 */
class ChartDataBuilder {
public:
    /// Sentinel category value meaning "do not filter by category".
    static const QString kAllCategories;

    /**
     * @brief Keep only the expenses that match @p filter.
     *
     * Filtering is deliberately a separate step so that the requirement
     * "filters are applied before aggregation" is explicit and testable.
     */
    static std::vector<Expense> applyFilter(const std::vector<Expense>& expenses,
                                            const ExpenseFilter& filter);

    /**
     * @brief Totals over time, for the line chart.
     * @return Pairs of (period-date, total amount) sorted ascending by date.
     *
     * For Period::Monthly each key date is the first day of the month.
     */
    static QVector<QPair<QDate, double>> totalsByPeriod(const std::vector<Expense>& expenses,
                                                        Period period);

    /**
     * @brief Totals per category, for the bar and pie charts.
     * @return Pairs of (category, total amount) sorted descending by amount.
     */
    static QVector<QPair<QString, double>> totalsByCategory(const std::vector<Expense>& expenses);
};

#endif // FINANCIAL_ASSISTANT_CHARTDATABUILDER_H
