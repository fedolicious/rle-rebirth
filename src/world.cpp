#include "world.h"

#include "raylib.h"

namespace world {

std::vector<aabb> platforms{
    // {3000,4000,2000,1000},
    {3000,12000,3000,300},
    // {9000,4000,2000,1500},
    // {9000,9000,3000,1000},
};

void draw(double scale) {
    for(const auto& p : platforms) {
        DrawRectangle(
            p.x/scale, p.y/scale,
            p.wdth/scale, p.hght/scale,
            GREEN);
    }
}
    
}