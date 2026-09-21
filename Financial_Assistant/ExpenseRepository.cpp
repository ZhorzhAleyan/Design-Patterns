#include "ExpenseRepository.h"

#include <utility>

#include <QDate>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <QVariant>

namespace {
// ---- Named constants (no "magic" literals scattered in the code) ----------
constexpr int    kCsvColumnCount   = 4;                 // date, category, amount, note
const QString    kDateFormat       = QStringLiteral("yyyy-MM-dd");
const QString    kConnPrefix       = QStringLiteral("expense_conn_");
}

ExpenseRepository::ExpenseRepository(QString dbPath)
    : m_dbPath(std::move(dbPath)) {
    // A unique connection name lets several repositories (e.g. in tests) coexist
    // without Qt complaining about a duplicate default connection.
    static int counter = 0;
    m_connectionName = kConnPrefix + QString::number(++counter);
}

ExpenseRepository::~ExpenseRepository() {
    // Close and drop the named connection so Qt does not warn about a
    // connection still in use at shutdown.
    if (m_db.isOpen())
        m_db.close();
    m_db = QSqlDatabase();  // release our reference before removing it
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool ExpenseRepository::open() {
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        setError(QStringLiteral("open database"), m_db.lastError().text());
        return false;
    }
    return createSchema();
}

bool ExpenseRepository::isOpen() const {
    return m_db.isOpen();
}

bool ExpenseRepository::createSchema() {
    QSqlQuery query(m_db);
    const QString ddl = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS expenses ("
        "  id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  date      TEXT    NOT NULL,"
        "  category  TEXT    NOT NULL,"
        "  amount    REAL    NOT NULL CHECK (amount >= 0),"
        "  note      TEXT"
        ")");
    if (!query.exec(ddl)) {
        setError(QStringLiteral("create schema"), query.lastError().text());
        return false;
    }
    return true;
}

std::vector<Expense> ExpenseRepository::getAll() {
    std::vector<Expense> expenses;
    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral(
            "SELECT id, date, category, amount, note FROM expenses ORDER BY date"))) {
        setError(QStringLiteral("select expenses"), query.lastError().text());
        return expenses;
    }

    while (query.next()) {
        Expense e;
        e.id       = query.value(0).toInt();
        e.date     = QDate::fromString(query.value(1).toString(), kDateFormat);
        e.category = query.value(2).toString();
        e.amount   = query.value(3).toDouble();
        e.note     = query.value(4).toString();
        expenses.push_back(e);
    }
    return expenses;
}

bool ExpenseRepository::add(Expense& expense) {
    // Parameterised query: prevents SQL injection and quoting mistakes.
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO expenses (date, category, amount, note) "
        "VALUES (:date, :category, :amount, :note)"));
    query.bindValue(QStringLiteral(":date"), expense.date.toString(kDateFormat));
    query.bindValue(QStringLiteral(":category"), expense.category);
    query.bindValue(QStringLiteral(":amount"), expense.amount);
    query.bindValue(QStringLiteral(":note"), expense.note);

    if (!query.exec()) {
        setError(QStringLiteral("insert expense"), query.lastError().text());
        return false;
    }
    expense.id = query.lastInsertId().toInt();  // report the generated id back
    return true;
}

bool ExpenseRepository::update(const Expense& expense) {
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "UPDATE expenses SET date = :date, category = :category, "
        "amount = :amount, note = :note WHERE id = :id"));
    query.bindValue(QStringLiteral(":date"), expense.date.toString(kDateFormat));
    query.bindValue(QStringLiteral(":category"), expense.category);
    query.bindValue(QStringLiteral(":amount"), expense.amount);
    query.bindValue(QStringLiteral(":note"), expense.note);
    query.bindValue(QStringLiteral(":id"), expense.id);

    if (!query.exec()) {
        setError(QStringLiteral("update expense"), query.lastError().text());
        return false;
    }
    return true;
}

bool ExpenseRepository::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("DELETE FROM expenses WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec()) {
        setError(QStringLiteral("delete expense"), query.lastError().text());
        return false;
    }
    return true;
}

bool ExpenseRepository::isEmpty() {
    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral("SELECT COUNT(*) FROM expenses")) || !query.next())
        return true;  // treat an unreadable table as empty for seeding purposes
    return query.value(0).toInt() == 0;
}

int ExpenseRepository::seedSampleData() {
    // A compact, realistic dataset spread across categories and months so all
    // three charts have something meaningful to show on first launch.
    std::vector<Expense> samples = {
        {0, QDate(2026, 1, 5),  QStringLiteral("Food"),      42.50, QStringLiteral("Groceries")},
        {0, QDate(2026, 1, 12), QStringLiteral("Transport"), 20.00, QStringLiteral("Metro card")},
        {0, QDate(2026, 1, 20), QStringLiteral("Food"),      15.75, QStringLiteral("Lunch")},
        {0, QDate(2026, 2, 3),  QStringLiteral("Rent"),     600.00, QStringLiteral("February rent")},
        {0, QDate(2026, 2, 14), QStringLiteral("Leisure"),   35.00, QStringLiteral("Cinema")},
        {0, QDate(2026, 2, 25), QStringLiteral("Food"),      58.20, QStringLiteral("Groceries")},
        {0, QDate(2026, 3, 1),  QStringLiteral("Rent"),     600.00, QStringLiteral("March rent")},
        {0, QDate(2026, 3, 9),  QStringLiteral("Transport"), 12.40, QStringLiteral("Taxi")},
        {0, QDate(2026, 3, 18), QStringLiteral("Leisure"),   80.00, QStringLiteral("Concert")},
        {0, QDate(2026, 3, 28), QStringLiteral("Food"),      47.90, QStringLiteral("Groceries")},
    };

    int inserted = 0;
    for (Expense& e : samples) {
        if (add(e))
            ++inserted;
    }
    return inserted;
}

// ---------------------------------------------------------------------------
// CSV import
// ---------------------------------------------------------------------------

namespace {
/**
 * @brief Split one CSV line into fields, honouring double-quoted values.
 *
 * Handles commas inside quotes and the "" escape for a literal quote. Kept
 * intentionally small — enough for well-formed export files.
 */
QStringList parseCsvLine(const QString& line) {
    QStringList fields;
    QString current;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); ++i) {
        const QChar c = line.at(i);
        if (inQuotes) {
            if (c == QLatin1Char('"')) {
                // A doubled quote ("") means one literal quote character.
                if (i + 1 < line.size() && line.at(i + 1) == QLatin1Char('"')) {
                    current.append(QLatin1Char('"'));
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                current.append(c);
            }
        } else {
            if (c == QLatin1Char('"')) {
                inQuotes = true;
            } else if (c == QLatin1Char(',')) {
                fields.append(current);
                current.clear();
            } else {
                current.append(c);
            }
        }
    }
    fields.append(current);
    return fields;
}
}  // namespace

int ExpenseRepository::importCsv(const QString& filePath, QStringList* skippedLog) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError(QStringLiteral("open CSV"), file.errorString());
        return -1;
    }

    QTextStream in(&file);
    int imported  = 0;
    int lineNo    = 0;
    bool headerSkipped = false;

    while (!in.atEnd()) {
        const QString rawLine = in.readLine();
        ++lineNo;

        if (rawLine.trimmed().isEmpty())
            continue;  // ignore blank lines silently

        // The first non-empty line is the header row (date,category,amount,note).
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        const QStringList fields = parseCsvLine(rawLine);
        if (fields.size() < kCsvColumnCount) {
            if (skippedLog)
                skippedLog->append(
                    QStringLiteral("Line %1: expected %2 columns, got %3")
                        .arg(lineNo).arg(kCsvColumnCount).arg(fields.size()));
            continue;
        }

        // Validate date.
        const QDate date = QDate::fromString(fields.at(0).trimmed(), kDateFormat);
        if (!date.isValid()) {
            if (skippedLog)
                skippedLog->append(QStringLiteral("Line %1: invalid date '%2'")
                                       .arg(lineNo).arg(fields.at(0).trimmed()));
            continue;
        }

        // Validate amount (must be numeric and non-negative).
        bool amountOk = false;
        const double amount = fields.at(2).trimmed().toDouble(&amountOk);
        if (!amountOk || amount < 0.0) {
            if (skippedLog)
                skippedLog->append(QStringLiteral("Line %1: invalid amount '%2'")
                                       .arg(lineNo).arg(fields.at(2).trimmed()));
            continue;
        }

        Expense e;
        e.date     = date;
        e.category = fields.at(1).trimmed();
        e.amount   = amount;
        e.note     = fields.at(3).trimmed();

        if (add(e)) {
            ++imported;
        } else if (skippedLog) {
            skippedLog->append(QStringLiteral("Line %1: database insert failed")
                                   .arg(lineNo));
        }
    }
    return imported;
}

void ExpenseRepository::setError(const QString& context, const QString& detail) {
    m_lastError = QStringLiteral("[%1] %2").arg(context, detail);
}
