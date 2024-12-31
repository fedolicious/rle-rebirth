#include "player.h"

#include "raylib.h"
#include "world.h"

#include <cmath>


namespace player {

/*
    TODO
    change collision from pos += vel to O(n) aabb cast from player to world platforms
    remove the half width/height hitbox offset
    add variables for chain hand offset
*/
double x = 200*16;
double y = 400*16;
double x_vel = 0;
double y_vel = 0;

bool on_ground() {
    return y == 12800.0f;
}
void tick(point chain_point, double chain_length) {
    bool outside;
    {
        const double x_dist = std::abs(player::x-chain_point.x);
        const double y_dist = std::abs(player::y-chain_point.y);
        outside = x_dist*x_dist + y_dist*y_dist > chain_length*chain_length;
    }
    //clamp within chain radius
    if(IsKeyDown(KEY_LEFT_SHIFT) && outside) {
        const auto rel_x = x-chain_point.x;
        const auto rel_y = y-chain_point.y;
        const auto chain_dist_sq = rel_x*rel_x + rel_y*rel_y;
        const auto chain_dist = std::sqrt(chain_dist_sq);
        x = chain_point.x + chain_length/chain_dist*rel_x;
        y = chain_point.y + chain_length/chain_dist*rel_y;
        const auto dot_prod = (x_vel*rel_x + y_vel*rel_y)/chain_dist_sq;
        x_vel -= rel_x*dot_prod;
        y_vel -= rel_y*dot_prod;
    }
    //velocity
    const double x_direction = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
    const auto target_vel = x_direction*max_ground_speed;
    if(on_ground()) {
        if(x_vel < target_vel) {
            x_vel = std::min(x_vel+ground_accel, target_vel);
        } else {
            x_vel = std::max(x_vel-ground_accel, target_vel);
        }
        if(IsKeyPressed(KEY_SPACE)) {
            y_vel = jump_vel;
        }
    } else {
        y_vel += gravity;
    }
    //position and clamp
    x += x_vel;
    y += y_vel;
    if(x > world::wdth) {
        x = world::wdth;
        x_vel = 0;
    } else if(x < 0) {
        x = 0;
        x_vel = 0;
    }
    if(y > world::hght) {
        y = world::hght;
        y_vel = 0;
    } else if(y < 0) {
        y = 0;
        y_vel = 0;
    }
}
void draw(double scale) {
    const auto drawnWdth = wdth/scale;
    const auto drawnHght = hght/scale;
    DrawRectangle(
        x/scale - drawnWdth/2,
        y/scale - drawnHght/2,
        drawnWdth,
        drawnHght,
        on_ground()?ORANGE:RED);
}

}
