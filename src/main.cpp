/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"    // utility header for SearchAndSetResourceDir

#include <cstdint>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

namespace world {
    constexpr double wdth = 1280*16;
    constexpr double hght = 800*16;
}
namespace player {
    constexpr double ground_accel = 10;
    constexpr double air_accel = 50;
    constexpr double gravity = 25;
    constexpr double jump_vel = -300;
    constexpr double max_ground_speed = 100;
    constexpr double wdth = 50*16;
    constexpr double hght = 50*16;
    double x = 200*16;
    double y = 400*16;
    double x_vel = 0;
    double y_vel = 0;
    // double x_accel = 0;
    // double y_accel = gravity;
    bool on_ground() {
        return y == 12800.0f;
    }
}
struct point {
    double x;
    double y;
};
int main () {
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(1280, 800, "Hello Raylib");

    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory
    Texture wabbit = LoadTexture("wabbit_alpha.png");
    
    // game loop
    constexpr point chain_point{640*16,400*16};
    constexpr double chain_length{400*16};
    while (!WindowShouldClose()) {
        bool outside;
        {
            const double x_dist = std::abs(player::x-chain_point.x);
            const double y_dist = std::abs(player::y-chain_point.y);
            outside = x_dist*x_dist + y_dist*y_dist > chain_length*chain_length;
        }
        //update player
        {
            using namespace player;
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
        BeginDrawing();
        constexpr uint32_t scale = 16;
        
        ClearBackground(BLACK);
        
        DrawText("Hello Raylib", 200, 200, 20, WHITE);
        DrawTexture(wabbit, 400, 200, WHITE);

        DrawText(TextFormat("x: %.0lf\ny: %.0lf\nxV: %.0lf\nyV: %.0lf", player::x, player::y, player::x_vel, player::y_vel),
            200, 250, 20, WHITE);

        DrawCircle(
            chain_point.x/scale,
            chain_point.y/scale,
            chain_length/scale,
            Color{255,255,255,uint8_t(outside?60:50)});
        DrawLine(
            player::x/scale,
            player::y/scale,
            chain_point.x/scale,
            chain_point.y/scale,
            ORANGE);
        const auto drawnWdth = player::wdth/scale;
        const auto drawnHght = player::hght/scale;
        DrawRectangle(
            player::x/scale - drawnWdth/2,
            player::y/scale - drawnHght/2,
            drawnWdth,
            drawnHght,
            player::on_ground()?ORANGE:RED);
        EndDrawing();
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
