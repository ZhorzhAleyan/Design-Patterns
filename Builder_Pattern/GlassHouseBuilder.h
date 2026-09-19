#pragma once

#include "HouseBuilder.h"

// ---------------------- ConcreteBuilder #2 ------------------
class GlassHouseBuilder : public HouseBuilder {
public:
    void buildWalls() override;
    void buildDoors() override;
    void buildWindows() override;
    void buildRoof() override;
    void buildGarage() override;
    void buildSwimmingPool() override;
};
