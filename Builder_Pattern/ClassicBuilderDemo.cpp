#include "ClassicBuilderDemo.h"

#include <iostream>
#include <memory>

#include "Engineer.h"
#include "GlassHouseBuilder.h"
#include "House.h"
#include "WoodenHouseBuilder.h"

void runClassicBuilderDemo() {
    Engineer engineer;

    WoodenHouseBuilder woodenBuilder;
    std::unique_ptr<House> woodenHouse = engineer.construct(woodenBuilder);

    GlassHouseBuilder glassBuilder;
    std::unique_ptr<House> glassHouse = engineer.construct(glassBuilder);

    std::cout << "--- Wooden house ---\n";
    woodenHouse->showSpecification();

    std::cout << "\n--- Glass house ---\n";
    glassHouse->showSpecification();
}
