#pragma once

#include <memory>

#include "House.h"

// ---------------------- Builder (interface) -----------------
// Declares the building steps that are common to every representation.
class HouseBuilder {
public:
    virtual ~HouseBuilder() = default;

    virtual void buildWalls() = 0;
    virtual void buildDoors() = 0;
    virtual void buildWindows() = 0;
    virtual void buildRoof() = 0;
    virtual void buildGarage() = 0;
    virtual void buildSwimmingPool() = 0;

    // Hands the finished product to the client.
    std::unique_ptr<House> getResult() { return std::move(house_); }

protected:
    // Each concrete builder starts with its own fresh Product instance.
    std::unique_ptr<House> house_ = std::make_unique<House>();
};
