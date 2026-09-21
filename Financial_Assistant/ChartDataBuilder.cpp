#include "ChartDataBuilder.h"

#include <algorithm>

#include <QMap>

// "All" is the label shown in the UI category combo box and the sentinel the
// logic layer recognises as "keep every category".
const QString ChartDataBuilder::kAllCategories = QStringLiteral("All");

std::vector<Expense> ChartDataBuilder::applyFilter(const std::vector<Expense>& expenses,
                                                   const ExpenseFilter& filter) {
    const bool filterByCategory =
        !filter.category.isEmpty() && filter.category != kAllCategories;

    std::vector<Expense> result;
    result.reserve(expenses.size());

    for (const Expense& e : expenses) {
        // Date lower bound (skip the check when 'from' is not set).
        if (filter.from.isValid() && e.date < filter.from)
            continue;
        // Date upper bound (skip the check when 'to' is not set).
        if (filter.to.isValid() && e.date > filter.to)
            continue;
        // Category match.
        if (filterByCategory && e.category != filter.category)
            continue;

        result.push_back(e);
    }
    return result;
}

QVector<QPair<QDate, double>> ChartDataBuilder::totalsByPeriod(const std::vector<Expense>& expenses,
                                                              Period period) {
    // QMap keeps keys sorted ascending, which is exactly what a time axis wants.
    QMap<QDate, double> totals;

    for (const Expense& e : expenses) {
        if (!e.date.isValid())
            continue;

        // For monthly grouping we collapse every date to the 1st of its month
        // so that all of, say, March lands under 2026-03-01.
        const QDate key = (period == Period::Monthly)
                              ? QDate(e.date.year(), e.date.month(), 1)
                              : e.date;

        totals[key] += e.amount;
    }

    QVector<QPair<QDate, double>> series;
    series.reserve(static_cast<int>(totals.size()));
    for (auto it = totals.cbegin(); it != totals.cend(); ++it)
        series.append({it.key(), it.value()});

    return series;
}

QVector<QPair<QString, double>> ChartDataBuilder::totalsByCategory(
    const std::vector<Expense>& expenses) {
    QMap<QString, double> totals;
    for (const Expense& e : expenses)
        totals[e.category] += e.amount;

    QVector<QPair<QString, double>> series;
    series.reserve(static_cast<int>(totals.size()));
    for (auto it = totals.cbegin(); it != totals.cend(); ++it)
        series.append({it.key(), it.value()});

    // Sort by amount descending so the biggest spenders read first in the
    // legend and the pie's largest slice is easy to find.
    std::sort(series.begin(), series.end(),
              [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
                  return a.second > b.second;
              });

    return series;
}
