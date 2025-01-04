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

bool on_ground() {
    for(const auto& platform : world::platforms) {
        if(aabb_to_point_over_aabb_trace(make_aabb(), {x,y+1}, platform).factor == 0) {
            return true;
        }
    }
    return false;
}
void tick() {
    const numeric_t held_x_direction = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
    if(IsKeyPressed(KEY_LEFT_SHIFT)) {
        if(current_chain.has_value()) {
            current_chain.reset();
        } else {
            const auto chain_base = point{x,y}+chain_offset;
            const trace_result res = world_trace(
                point_to_point_over_aabb_trace,
                chain_base,
                point{chain_base.x + held_x_direction*chain_range, chain_base.y});
            std::cout
                << res.end_point.x << ", "
                << res.end_point.y << ", "
                << int(res.hit_side) << ", "
                << res.factor << "\n";
            if(res.hit_side != trace_result::side::none) {
                const auto offset = point{x,y} - res.end_point;
                // const auto chain_length = std::sqrt(offset.norm);
                current_chain.emplace(res.end_point, std::sqrt(offset.norm()));
            }
        }
    }
    //chain kinematics
    if(current_chain.has_value()) {
        const auto& attach_point = current_chain->attach_point;
        const auto relative_pos = point{x,y} - attach_point + chain_offset;
        
        const auto chain_dist_sq = relative_pos.x*relative_pos.x + relative_pos.y*relative_pos.y;
        const auto chain_dist = std::sqrt(chain_dist_sq);
        if(chain_dist >= current_chain->length) {
            const auto new_pos = attach_point - chain_offset + current_chain->length/chain_dist*relative_pos;
            x = new_pos.x;
            y = new_pos.y;
            // x = chain_point.x + chain_length/chain_dist*relative_pos.x;
            // y = chain_point.y + chain_length/chain_dist*relative_pos.y;
            const auto dot_prod = (x_vel*relative_pos.x + y_vel*relative_pos.y)/chain_dist_sq;
            const auto new_vel = point{x_vel,y_vel} - relative_pos*dot_prod;
            x_vel = new_vel.x;
            y_vel = new_vel.y;
        }
    }
    //player movement kinematics
    const auto target_vel = held_x_direction*max_ground_speed;
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
    DrawRectangle(
        x/scale,
        y/scale,
        wdth/scale,
        hght/scale,
        on_ground()?ORANGE:RED);
    const auto chain_base = point{x,y}+chain_offset;
    DrawLine(
        chain_base.x/scale,
        chain_base.y/scale,
        (chain_base.x + (IsKeyDown(KEY_D)-IsKeyDown(KEY_A))*chain_range)/scale,
        chain_base.y/scale,
        ORANGE);
    if(current_chain.has_value()) {
        const auto drawn_x = current_chain->attach_point.x/scale;
        const auto drawn_y = current_chain->attach_point.y/scale;
        DrawCircle(
            drawn_x, drawn_y,
            current_chain->length/scale,
            Color{255,255,255,uint8_t(false?60:50)});
        DrawLine(
            drawn_x, drawn_y,
            current_chain->attach_point.x/scale,
            current_chain->attach_point.y/scale,
            ORANGE);
    }
}
aabb make_aabb() { return {x,y,wdth,hght}; }

}
