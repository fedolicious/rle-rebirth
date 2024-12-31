#pragma once

#include "common.h"

namespace player {
    constexpr double ground_accel = 10;
    constexpr double air_accel = 50;
    constexpr double gravity = 25;
    constexpr double jump_vel = -300;
    constexpr double max_ground_speed = 100;
    constexpr double wdth = 50*16;
    constexpr double hght = 50*16;
    extern double x;
    extern double y;
    extern double x_vel;
    extern double y_vel;
    
    bool on_ground();
    void tick(point chain_point, double chain_length);
    void draw(double scale);
}
