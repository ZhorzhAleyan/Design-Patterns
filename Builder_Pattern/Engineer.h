#pragma once

#include <memory>

#include "House.h"
#include "HouseBuilder.h"

// ---------------------- Director -----------------------------
// Knows the correct order of steps required to build a valid house,
// but knows nothing about how each step is actually implemented.
class Engineer {
public:
    std::unique_ptr<House> construct(HouseBuilder& builder) const;
};
