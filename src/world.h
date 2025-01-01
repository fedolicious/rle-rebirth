#pragma once

#include "geometry.h"
#include "vector"

namespace world {
    constexpr double wdth = 1280*16;
    constexpr double hght = 800*16;
    extern std::vector<aabb> platforms;
    
    void draw(double scale);
}
