#include <iostream>

#include "ClassicBuilderDemo.h"
#include "FluentBuilderDemo.h"

// ============================================================
// BUILDER DESIGN PATTERN — two variants for comparison
// ------------------------------------------------------------
// Variant 1: classic GoF Builder — House.h/.cpp, HouseBuilder.h,
// WoodenHouseBuilder.h/.cpp, GlassHouseBuilder.h/.cpp, Engineer.h/.cpp.
//
// Variant 2: fluent (modern) Builder — Pizza.h/.cpp.
// ============================================================
int main() {
    std::cout << "===== Classic GoF Builder (House) =====\n\n";
    runClassicBuilderDemo();

    std::cout << "\n===== Fluent Builder (Pizza) =====\n\n";
    runFluentBuilderDemo();

    return 0;
}
