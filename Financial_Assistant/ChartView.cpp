#include "ChartView.h"

#include <algorithm>

#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QDateEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSet>
#include <QToolTip>
#include <QVBoxLayout>

#include "AddExpenseDialog.h"

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QValueAxis>

namespace {
// ---- Named constants -------------------------------------------------------
// Chart-type combo box indices.
constexpr int kChartLine = 0;
constexpr int kChartBar  = 1;
constexpr int kChartPie  = 2;

constexpr int    kMaxLabelLength   = 15;    // truncate category labels beyond this
const QString    kEllipsis         = QStringLiteral("...");
const QString    kUiDateFormat     = QStringLiteral("yyyy-MM-dd");
const QString    kMonthAxisFormat  = QStringLiteral("MMM yyyy");
constexpr double kAxisHeadroom     = 1.10;  // 10% empty space above the tallest value
constexpr double kExplodeDistance  = 0.10;  // how far the largest pie slice pops out
constexpr int    kFallbackRangeDays = 30;   // date span used when there is no data
constexpr double kPieSizeFactor    = 0.70;  // shrink the pie so outside labels have room
constexpr double kMinLabelPercent  = 3.0;   // hide the on-slice label below this share (%)
const QColor     kBarLabelColor    = QColor(60, 60, 60);  // readable on the light background
}  // namespace

ChartView::ChartView(QWidget* parent) : QWidget(parent) {
    buildControls();
}

// ---------------------------------------------------------------------------
// Layout & controls
// ---------------------------------------------------------------------------

void ChartView::buildControls() {
    // --- Filter/toolbar row ---
    m_chartTypeCombo = new QComboBox(this);
    m_chartTypeCombo->setObjectName(QStringLiteral("chartTypeCombo"));  // used for QSS/tests
    m_chartTypeCombo->addItem(tr("Line"), kChartLine);
    m_chartTypeCombo->addItem(tr("Bar"),  kChartBar);
    m_chartTypeCombo->addItem(tr("Pie"),  kChartPie);

    m_fromDateEdit = new QDateEdit(this);
    m_fromDateEdit->setCalendarPopup(true);
    m_fromDateEdit->setDisplayFormat(kUiDateFormat);

    m_toDateEdit = new QDateEdit(this);
    m_toDateEdit->setCalendarPopup(true);
    m_toDateEdit->setDisplayFormat(kUiDateFormat);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setObjectName(QStringLiteral("categoryCombo"));  // used for QSS/tests
    m_categoryCombo->addItem(ChartDataBuilder::kAllCategories);

    m_addButton    = new QPushButton(tr("Add Expense..."), this);
    m_importButton = new QPushButton(tr("Import CSV..."), this);

    auto* controls = new QHBoxLayout;
    controls->addWidget(new QLabel(tr("Chart:"), this));
    controls->addWidget(m_chartTypeCombo);
    controls->addSpacing(12);
    controls->addWidget(new QLabel(tr("From:"), this));
    controls->addWidget(m_fromDateEdit);
    controls->addWidget(new QLabel(tr("To:"), this));
    controls->addWidget(m_toDateEdit);
    controls->addSpacing(12);
    controls->addWidget(new QLabel(tr("Category:"), this));
    controls->addWidget(m_categoryCombo);
    controls->addStretch();
    controls->addWidget(m_addButton);
    controls->addWidget(m_importButton);

    // --- Chart area (a chart view and a message label share the same slot) ---
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #888; font-size: 16px;"));
    m_statusLabel->hide();

    auto* root = new QVBoxLayout(this);
    root->addLayout(controls);
    root->addWidget(m_chartView, /*stretch=*/1);
    root->addWidget(m_statusLabel, /*stretch=*/1);

    // --- Wiring: any change re-renders the chart ---
    connect(m_chartTypeCombo, &QComboBox::currentIndexChanged,
            this, &ChartView::onFilterChanged);
    connect(m_categoryCombo, &QComboBox::currentIndexChanged,
            this, &ChartView::onFilterChanged);
    connect(m_fromDateEdit, &QDateEdit::dateChanged, this, &ChartView::onFilterChanged);
    connect(m_toDateEdit, &QDateEdit::dateChanged, this, &ChartView::onFilterChanged);
    connect(m_addButton, &QPushButton::clicked, this, &ChartView::onAddButtonClicked);
    connect(m_importButton, &QPushButton::clicked, this, &ChartView::onImportButtonClicked);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ChartView::setExpenses(const std::vector<Expense>& expenses) {
    m_expenses = expenses;

    // Adjust the date pickers to span the data. Block signals so these
    // programmatic changes do not trigger a redraw for every call.
    QDate minDate, maxDate;
    for (const Expense& e : m_expenses) {
        if (!e.date.isValid())
            continue;
        if (!minDate.isValid() || e.date < minDate) minDate = e.date;
        if (!maxDate.isValid() || e.date > maxDate) maxDate = e.date;
    }
    if (!minDate.isValid()) {
        // No data: fall back to a sensible window ending today.
        maxDate = QDate::currentDate();
        minDate = maxDate.addDays(-kFallbackRangeDays);
    }

    const QSignalBlocker blockFrom(m_fromDateEdit);
    const QSignalBlocker blockTo(m_toDateEdit);
    // Allow the pickers to roam a little beyond the data on both sides.
    m_fromDateEdit->setDateRange(minDate.addYears(-1), maxDate.addYears(1));
    m_toDateEdit->setDateRange(minDate.addYears(-1), maxDate.addYears(1));
    m_fromDateEdit->setDate(minDate);
    m_toDateEdit->setDate(maxDate);

    refreshCategoryCombo();
    rebuildChart();
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------

void ChartView::onFilterChanged() {
    rebuildChart();
}

void ChartView::onImportButtonClicked() {
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Import expenses from CSV"), QString(),
        tr("CSV files (*.csv);;All files (*)"));
    if (!path.isEmpty())
        emit importCsvRequested(path);  // the owner performs the DB import + reload
}

void ChartView::onAddButtonClicked() {
    // Offer the categories already present so the user can reuse one or add new.
    QStringList categories;
    for (int i = 0; i < m_categoryCombo->count(); ++i) {
        const QString text = m_categoryCombo->itemText(i);
        if (text != ChartDataBuilder::kAllCategories)  // skip the "All" filter entry
            categories << text;
    }

    AddExpenseDialog dialog(categories, this);
    if (dialog.exec() == QDialog::Accepted)
        emit addExpenseRequested(dialog.expense());  // owner saves it via the repository
}

// ---------------------------------------------------------------------------
// Chart building
// ---------------------------------------------------------------------------

ExpenseFilter ChartView::currentFilter() const {
    ExpenseFilter filter;
    filter.from     = m_fromDateEdit->date();
    filter.to       = m_toDateEdit->date();
    filter.category = m_categoryCombo->currentText();
    return filter;
}

void ChartView::rebuildChart() {
    // Edge case: an inverted date range is a user error, not a crash.
    if (m_fromDateEdit->date() > m_toDateEdit->date()) {
        showMessage(tr("Invalid date range:\nthe 'From' date is after the 'To' date."));
        return;
    }

    // Filtering happens in the logic layer, before any aggregation.
    const std::vector<Expense> data =
        ChartDataBuilder::applyFilter(m_expenses, currentFilter());

    // Edge case: nothing to show (either no data at all, or none after filtering).
    if (data.empty()) {
        showMessage(m_expenses.empty() ? tr("No data to display.")
                                       : tr("No expenses match the current filters."));
        return;
    }

    switch (m_chartTypeCombo->currentData().toInt()) {
        case kChartBar: showBarChart(data);  break;
        case kChartPie: showPieChart(data);  break;
        case kChartLine:
        default:        showLineChart(data); break;
    }
}

void ChartView::showLineChart(const std::vector<Expense>& data) {
    // The line chart shows the monthly spending trend over time.
    const auto totals = ChartDataBuilder::totalsByPeriod(data, Period::Monthly);

    auto* series = new QLineSeries;
    series->setName(tr("Monthly total"));
    series->setPointsVisible(true);

    double maxValue = 0.0;
    for (const auto& point : totals) {
        const qint64 x = point.first.startOfDay().toMSecsSinceEpoch();
        series->append(static_cast<double>(x), point.second);
        maxValue = std::max(maxValue, point.second);
    }

    auto* chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(tr("Spending over time"));
    chart->legend()->hide();

    auto* axisX = new QDateTimeAxis;
    axisX->setFormat(kMonthAxisFormat);
    axisX->setTitleText(tr("Month"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis;
    axisY->setRange(0, maxValue * kAxisHeadroom);
    axisY->setTitleText(tr("Amount"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Hover over a point -> show its exact date and value as a tooltip.
    connect(series, &QLineSeries::hovered, this,
            [](const QPointF& pt, bool state) {
                if (state) {
                    const QDate d =
                        QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(pt.x())).date();
                    QToolTip::showText(
                        QCursor::pos(),
                        QStringLiteral("%1: %2")
                            .arg(d.toString(kUiDateFormat))
                            .arg(pt.y(), 0, 'f', 2));
                } else {
                    QToolTip::hideText();
                }
            });

    m_chartView->setChart(chart);   // QChartView takes ownership of the chart
    m_statusLabel->hide();
    m_chartView->show();
}

void ChartView::showBarChart(const std::vector<Expense>& data) {
    // Each bar is one category's total.
    const auto totals = ChartDataBuilder::totalsByCategory(data);

    auto* set = new QBarSet(tr("Spending"));
    // The default bar-label colour is white, which is invisible on the light
    // plot background, so give the on-bar amount labels a readable dark colour.
    set->setLabelColor(kBarLabelColor);
    QStringList categories;
    double maxValue = 0.0;
    for (const auto& item : totals) {
        *set << item.second;
        categories << truncateLabel(item.first);
        maxValue = std::max(maxValue, item.second);
    }

    auto* series = new QBarSeries;
    series->append(set);
    series->setLabelsVisible(true);  // print the amount on each bar
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    auto* chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(tr("Spending by category"));
    chart->legend()->hide();

    auto* axisX = new QBarCategoryAxis;
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis;
    axisY->setRange(0, maxValue * kAxisHeadroom);
    axisY->setTitleText(tr("Amount"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chartView->setChart(chart);
    m_statusLabel->hide();
    m_chartView->show();
}

void ChartView::showPieChart(const std::vector<Expense>& data) {
    const auto totals = ChartDataBuilder::totalsByCategory(data);

    double grandTotal = 0.0;
    for (const auto& item : totals)
        grandTotal += item.second;

    auto* series = new QPieSeries;
    // Shrink the pie so there is room around it for the outside labels.
    series->setPieSize(kPieSizeFactor);

    for (const auto& item : totals) {
        const double percent = (grandTotal > 0.0) ? (item.second / grandTotal * 100.0) : 0.0;
        QPieSlice* slice = series->append(truncateLabel(item.first), item.second);

        // Label shows the category and its share of total spending.
        slice->setLabel(QStringLiteral("%1 (%2%)")
                            .arg(truncateLabel(item.first))
                            .arg(percent, 0, 'f', 1));

        // Place labels OUTSIDE the pie with connector arms. Inside labels of
        // small or adjacent slices pile up in the same spot near the centre;
        // outside labels are spread around the rim, each on its own arm.
        slice->setLabelPosition(QPieSlice::LabelOutside);

        // A very thin slice still can't fit a readable arm without colliding
        // with its neighbour, so hide its on-slice label. The category is not
        // lost: it remains in the legend on the right (and on hover).
        slice->setLabelVisible(percent >= kMinLabelPercent);
    }

    // totalsByCategory returns slices sorted largest-first, so the first slice
    // is the biggest; pop it out slightly for emphasis.
    if (!series->slices().isEmpty()) {
        QPieSlice* largest = series->slices().first();
        largest->setExploded(true);
        largest->setExplodeDistanceFactor(kExplodeDistance);
    }

    auto* chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(tr("Category share of total spending"));
    chart->legend()->setAlignment(Qt::AlignRight);

    m_chartView->setChart(chart);
    m_statusLabel->hide();
    m_chartView->show();
}

void ChartView::showMessage(const QString& text) {
    m_statusLabel->setText(text);
    m_chartView->hide();
    m_statusLabel->show();
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void ChartView::refreshCategoryCombo() {
    // Remember the current choice so we can restore it after rebuilding.
    const QString previous = m_categoryCombo->currentText();

    QSet<QString> uniqueCategories;
    for (const Expense& e : m_expenses)
        uniqueCategories.insert(e.category);

    QStringList sorted(uniqueCategories.cbegin(), uniqueCategories.cend());
    std::sort(sorted.begin(), sorted.end());

    const QSignalBlocker blocker(m_categoryCombo);  // avoid a redraw mid-rebuild
    m_categoryCombo->clear();
    m_categoryCombo->addItem(ChartDataBuilder::kAllCategories);
    m_categoryCombo->addItems(sorted);

    const int idx = m_categoryCombo->findText(previous);
    m_categoryCombo->setCurrentIndex(idx >= 0 ? idx : 0);
}

QString ChartView::truncateLabel(const QString& text) {
    if (text.length() <= kMaxLabelLength)
        return text;
    return text.left(kMaxLabelLength - kEllipsis.length()) + kEllipsis;
}
