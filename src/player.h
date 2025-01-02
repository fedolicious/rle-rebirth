#pragma once

#include "geometry.h"
#include "world.h"

namespace player {
    using numeric_t = double;
    constexpr numeric_t ground_accel = 10;
    constexpr numeric_t air_accel = 50;
    constexpr numeric_t gravity = 25;
    constexpr numeric_t jump_vel = -300;
    constexpr numeric_t max_ground_speed = 100;
    constexpr numeric_t wdth = 50*16;
    constexpr numeric_t hght = 50*16;
    constexpr numeric_t max_x = world::wdth - wdth;
    constexpr numeric_t max_y = world::hght - hght;
    extern numeric_t x;
    extern numeric_t y;
    extern numeric_t x_vel;
    extern numeric_t y_vel;
    
    bool on_ground();
    void tick(point chain_point, numeric_t chain_length);
    void draw(numeric_t scale);
    aabb make_aabb();
}
