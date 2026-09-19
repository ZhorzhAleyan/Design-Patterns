#include "Engineer.h"

std::unique_ptr<House> Engineer::construct(HouseBuilder& builder) const {
    builder.buildWalls();
    builder.buildDoors();
    builder.buildWindows();
    builder.buildRoof();
    builder.buildGarage();
    builder.buildSwimmingPool();
    return builder.getResult();
}
