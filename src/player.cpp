#include "player.h"

#include "raylib.h"
#include "world.h"

#include <cmath>
#include <iostream>
#include <cassert>

namespace player {

/*
    TODO
    add variables for chain hand offset
*/
numeric_t x = 200*16;
numeric_t y = 400*16;
numeric_t x_vel = 0;
numeric_t y_vel = 0;

bool on_ground() {
    for(const auto& platform : world::platforms) {
        if(aabb_to_point_over_aabb_trace(make_aabb(), {x,y+1}, platform).factor == 0) {
            return true;
        }
    }
    return false;
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
        if constexpr(can_fly) {
            if(IsKeyPressed(KEY_SPACE)) {
                y_vel = jump_vel;
            }
        }
        //TODO swinging should affect movement when not on ground
        y_vel += gravity;
    }
    //collide and slide
    point target_pos;
    size_t i = 0;
    do {
        const aabb player = make_aabb();
        target_pos = {x+x_vel, y+y_vel};
        
        trace_result world_trace = {target_pos, 1, trace_result::side::none};
        for(const auto& platform : world::platforms) {
            const auto platform_trace = aabb_to_point_over_aabb_trace(player, target_pos, platform);
            if(platform_trace.factor < world_trace.factor) {
                world_trace = platform_trace;
            }
        }
        
        x = world_trace.end_point.x;
        y = world_trace.end_point.y;
        
        switch(world_trace.hit_side) {
        using enum trace_result::side;
            case neg_x:
            case pos_x:
                x_vel = 0;
                break;
            case neg_y:
            case pos_y:
                y_vel = 0;
                break;
            case none: break;
        }
        
        assert(i < 10);
        i++;
    } while(point{x,y} != target_pos);
}
void draw(double scale) {
    const auto drawnWdth = wdth/scale;
    const auto drawnHght = hght/scale;
    DrawRectangle(
        x/scale,
        y/scale,
        drawnWdth,
        drawnHght,
        on_ground()?ORANGE:RED);
}
aabb make_aabb() { return {x,y,wdth,hght}; }

}
