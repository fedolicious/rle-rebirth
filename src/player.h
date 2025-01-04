#pragma once

#include "geometry.h"
#include "world.h"

#include <optional>

struct chain {
    point attach_point;
    point::numeric_t length;
};

namespace player {
    using numeric_t = double;
    constexpr numeric_t ground_accel = 10;
    constexpr numeric_t air_accel = 50;
    constexpr numeric_t gravity = 25;
    constexpr numeric_t jump_vel = -300;
    constexpr numeric_t max_ground_speed = 100;
    constexpr numeric_t wdth = 50*16;
    constexpr numeric_t hght = 50*16;
    constexpr numeric_t chain_range = 400*16;
    constexpr bool can_fly = false;
    inline numeric_t x = 200*16;
    inline numeric_t y = 400*16;
    inline numeric_t x_vel = 0;
    inline numeric_t y_vel = 0;
    inline point chain_offset{wdth/2, 0};
    inline std::optional<chain> current_chain{std::nullopt};
    
    bool on_ground();
    void tick();
    void draw(numeric_t scale);
    aabb make_aabb();
}
