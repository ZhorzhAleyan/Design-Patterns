#ifndef FINANCIAL_ASSISTANT_EXPENSEREPOSITORY_H
#define FINANCIAL_ASSISTANT_EXPENSEREPOSITORY_H

#include <vector>

#include <QSqlDatabase>
#include <QString>
#include <QStringList>

#include "Expense.h"

/**
 * @brief Data layer: the single gateway to persistent expense storage.
 *
 * Responsibilities:
 *   - own the SQLite connection and create the schema,
 *   - CRUD operations (create / read / update / delete),
 *   - import expenses from a CSV file,
 *   - seed sample data for the demo.
 *
 * The rest of the application only ever sees std::vector<Expense>; no SQL
 * leaks out of this class. That is what lets the UI stay database-agnostic.
 */
class ExpenseRepository {
public:
    /**
     * @param dbPath Path to the SQLite database file (created if missing).
     */
    explicit ExpenseRepository(QString dbPath);
    ~ExpenseRepository();

    // Non-copyable: it owns a database connection.
    ExpenseRepository(const ExpenseRepository&) = delete;
    ExpenseRepository& operator=(const ExpenseRepository&) = delete;

    /// Open the connection and ensure the schema exists. Returns false on error.
    bool open();
    bool isOpen() const;

    /// Human-readable text of the most recent failure (empty if none).
    QString lastError() const { return m_lastError; }

    // ---- CRUD ----------------------------------------------------------
    std::vector<Expense> getAll();                 ///< All rows, ordered by date.
    bool add(Expense& expense);                    ///< Inserts; fills expense.id on success.
    bool update(const Expense& expense);           ///< Updates the row with expense.id.
    bool remove(int id);                           ///< Deletes the row with the given id.

    // ---- Bulk / helpers ------------------------------------------------
    bool isEmpty();                                ///< True if the table has no rows.
    int  seedSampleData();                         ///< Inserts demo rows; returns count added.

    /**
     * @brief Import expenses from a CSV file into the database.
     *
     * Expected columns (header row required): date,category,amount,note
     * Date must be YYYY-MM-DD and amount must be a number >= 0. Rows that fail
     * validation are skipped (never abort the whole import) and appended to
     * @p skippedLog with a reason.
     *
     * @return Number of rows successfully imported, or -1 if the file could not
     *         be opened.
     */
    int importCsv(const QString& filePath, QStringList* skippedLog = nullptr);

private:
    QString      m_dbPath;
    QString      m_connectionName;  ///< Unique name so multiple repos don't clash.
    QSqlDatabase m_db;
    QString      m_lastError;

    bool createSchema();
    void setError(const QString& context, const QString& detail);
};

#endif // FINANCIAL_ASSISTANT_EXPENSEREPOSITORY_H
