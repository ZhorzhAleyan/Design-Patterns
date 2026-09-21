#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStringList>

#include "ChartView.h"
#include "ExpenseRepository.h"

/**
 * @brief Small demo that wires the three layers together:
 *
 *   ExpenseRepository (data)  ->  ChartView (UI, uses ChartDataBuilder logic)
 *
 * The repository opens (or creates) a SQLite database, seeds it with sample
 * data on first launch, and the chart view visualises it. CSV import is routed
 * back through the repository so the UI never touches SQL directly.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Store the database next to the user's app data so re-runs keep the data.
    const QString dataDir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    const QString dbPath = QDir(dataDir).filePath(QStringLiteral("expenses.db"));

    ExpenseRepository repository(dbPath);
    if (!repository.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Database error"),
                              QObject::tr("Could not open the database:\n%1")
                                  .arg(repository.lastError()));
        return 1;
    }

    // First run: populate a few sample expenses so the charts are not empty.
    if (repository.isEmpty())
        repository.seedSampleData();

    ChartView view;
    view.setWindowTitle(QObject::tr("Expense Tracker - Visualization"));
    view.resize(900, 600);
    view.setExpenses(repository.getAll());

    // When the user imports a CSV, the repository does the work and the view
    // is refreshed with the new dataset.
    QObject::connect(&view, &ChartView::importCsvRequested,
                     [&](const QString& path) {
                         QStringList skipped;
                         const int imported = repository.importCsv(path, &skipped);
                         if (imported < 0) {
                             QMessageBox::warning(
                                 &view, QObject::tr("Import failed"),
                                 QObject::tr("Could not read the file:\n%1")
                                     .arg(repository.lastError()));
                             return;
                         }
                         view.setExpenses(repository.getAll());  // refresh the chart

                         QString msg = QObject::tr("Imported %1 row(s).").arg(imported);
                         if (!skipped.isEmpty())
                             msg += QObject::tr("\nSkipped %1 invalid row(s):\n%2")
                                        .arg(skipped.size())
                                        .arg(skipped.join(QLatin1Char('\n')));
                         QMessageBox::information(&view, QObject::tr("CSV import"), msg);
                     });

    // Manual "Add expense" form: the repository stores it, then the chart refreshes.
    QObject::connect(&view, &ChartView::addExpenseRequested,
                     [&](const Expense& expense) {
                         Expense toStore = expense;  // add() fills in the generated id
                         if (!repository.add(toStore)) {
                             QMessageBox::warning(
                                 &view, QObject::tr("Could not add expense"),
                                 repository.lastError());
                             return;
                         }
                         view.setExpenses(repository.getAll());  // refresh the chart
                     });

    view.show();
    return app.exec();
}
