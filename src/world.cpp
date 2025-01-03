#include "world.h"

#include "raylib.h"

namespace world {

std::vector<aabb> platforms{
    //world border
    {0,0,wdth,0},
    {0,hght,wdth,0},
    {0,0,0,hght},
    {wdth,0,0,hght},
    
    {3000,12000,3000,300},
    {9000,4000,2000,1500},
    {9000,9000,3000,1000},
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