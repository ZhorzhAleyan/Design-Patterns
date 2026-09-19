#include "WoodenHouseBuilder.h"

void WoodenHouseBuilder::buildWalls() { house_->setWalls("wooden planks"); }
void WoodenHouseBuilder::buildDoors() { house_->setDoors("oak wood door"); }
void WoodenHouseBuilder::buildWindows() { house_->setWindows("small wooden-framed windows"); }
void WoodenHouseBuilder::buildRoof() { house_->setRoof("wooden shingle roof"); }
void WoodenHouseBuilder::buildGarage() { house_->setGarage(false); }
void WoodenHouseBuilder::buildSwimmingPool() { house_->setSwimmingPool(false); }
