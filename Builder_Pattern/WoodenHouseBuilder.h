#pragma once

#include "HouseBuilder.h"

// ---------------------- ConcreteBuilder #1 ------------------
class WoodenHouseBuilder : public HouseBuilder {
public:
    void buildWalls() override;
    void buildDoors() override;
    void buildWindows() override;
    void buildRoof() override;
    void buildGarage() override;
    void buildSwimmingPool() override;
};
