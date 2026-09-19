#include "GlassHouseBuilder.h"

void GlassHouseBuilder::buildWalls() { house_->setWalls("reinforced glass panels"); }
void GlassHouseBuilder::buildDoors() { house_->setDoors("sliding glass door"); }
void GlassHouseBuilder::buildWindows() { house_->setWindows("floor-to-ceiling glass windows"); }
void GlassHouseBuilder::buildRoof() { house_->setRoof("flat glass-tiled roof"); }
void GlassHouseBuilder::buildGarage() { house_->setGarage(true); }
void GlassHouseBuilder::buildSwimmingPool() { house_->setSwimmingPool(true); }
