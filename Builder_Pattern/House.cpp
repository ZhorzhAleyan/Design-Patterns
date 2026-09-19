#include "House.h"

#include <iostream>

void House::setWalls(const std::string& walls) { walls_ = walls; }
void House::setDoors(const std::string& doors) { doors_ = doors; }
void House::setWindows(const std::string& windows) { windows_ = windows; }
void House::setRoof(const std::string& roof) { roof_ = roof; }
void House::setGarage(bool hasGarage) { hasGarage_ = hasGarage; }
void House::setSwimmingPool(bool hasPool) { hasPool_ = hasPool; }

void House::showSpecification() const {
    std::cout << "House specification:\n";
    std::cout << "  Walls        : " << walls_ << "\n";
    std::cout << "  Doors        : " << doors_ << "\n";
    std::cout << "  Windows      : " << windows_ << "\n";
    std::cout << "  Roof         : " << roof_ << "\n";
    std::cout << "  Garage       : " << (hasGarage_ ? "yes" : "no") << "\n";
    std::cout << "  Swimming pool: " << (hasPool_ ? "yes" : "no") << "\n";
}
