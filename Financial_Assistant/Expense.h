#ifndef FINANCIAL_ASSISTANT_EXPENSE_H
#define FINANCIAL_ASSISTANT_EXPENSE_H

#include <QDate>
#include <QString>

/**
 * @brief A single expense record.
 *
 * This is a plain data-transfer object (DTO). It carries no logic and knows
 * nothing about where it came from (database, CSV file, or a manual form).
 * Every layer of the application speaks in terms of Expense objects, which is
 * what keeps the visualization code independent from the data source.
 */
struct Expense {
    int     id       = 0;   ///< Unique id assigned by the database (0 = not yet stored).
    QDate   date;           ///< Date on which the expense occurred.
    QString category;       ///< Category, e.g. "Food", "Transport".
    double  amount   = 0.0; ///< Amount spent, always >= 0.
    QString note;           ///< Optional free-text description.
};

#endif // FINANCIAL_ASSISTANT_EXPENSE_H
