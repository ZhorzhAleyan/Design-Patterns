#pragma once

#include <string>

// ---------------------- Product ----------------------------
// The complex object being built, piece by piece.
class House {
public:
    void setWalls(const std::string& walls);
    void setDoors(const std::string& doors);
    void setWindows(const std::string& windows);
    void setRoof(const std::string& roof);
    void setGarage(bool hasGarage);
    void setSwimmingPool(bool hasPool);

    void showSpecification() const;

private:
    std::string walls_;
    std::string doors_;
    std::string windows_;
    std::string roof_;
    bool hasGarage_ = false;
    bool hasPool_ = false;
};
