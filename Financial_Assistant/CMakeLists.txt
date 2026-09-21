cmake_minimum_required(VERSION 3.16)
project(Financial_Assistant LANGUAGES CXX)

# --- C++ standard --------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt automatic tooling ------------------------------------------------
# AUTOMOC runs moc on classes with Q_OBJECT (ChartView). AUTORCC/AUTOUIC are
# harmless to leave on for future resource/UI files.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- Qt 6 modules --------------------------------------------------------
# Widgets: the GUI. Charts: line/bar/pie. Sql: the SQLite database access.
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Sql)

add_executable(Financial_Assistant
        main.cpp
        Expense.h
        ExpenseRepository.h
        ExpenseRepository.cpp
        ChartDataBuilder.h
        ChartDataBuilder.cpp
        ChartView.h
        ChartView.cpp
        AddExpenseDialog.h
        AddExpenseDialog.cpp
)

target_link_libraries(Financial_Assistant
        PRIVATE
        Qt6::Widgets
        Qt6::Charts
        Qt6::Sql
)
