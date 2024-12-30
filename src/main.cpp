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
#include <algorithm>
#include <limits>
#include <iostream>

namespace world {
    constexpr uint32_t wdth = 1280*16;
    constexpr uint32_t hght = 800*16;
}
namespace player { //TODO switch to float, then switch to fixed point
    constexpr uint32_t speed = 1;
    constexpr int32_t gravity = 25;
    constexpr int32_t max_y_vel = 30*16;
    constexpr uint32_t wdth = 50*16;
    constexpr uint32_t hght = 50*16;
    uint32_t x = 200*16;
    uint32_t y = 400*16;
    int32_t x_vel = 0;
    int32_t y_vel = 0;
    int32_t x_accel = 0;
    int32_t y_accel = gravity;
}
struct point {
    uint32_t x;
    uint32_t y;
};

enum class overflow : uint8_t {none, high, low};
[[nodiscard]] constexpr overflow sum_overflow(uint32_t x, int32_t y) {
    using enum overflow;
    if(y > 0) {
        return (x > std::numeric_limits<decltype(x)>::max() - y) ? high : none;
    } else {
        return (x < std::numeric_limits<decltype(x)>::min() - y) ? low : none;
    }
}
template<class T1, class T2>
overflow sum_overflow(T1, T2) = delete;

template<std::unsigned_integral T>
constexpr T distance(T x, T y) {
    return (x > y) ? x-y : y-x;
}

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
    constexpr uint32_t chain_length{400*16};
    while (!WindowShouldClose()) {
        //update player
        bool outside;
        {
            // const x_dist = (player::x > chain_point.x) ? player::x-chain_point.x : chain_point.x-player::x;
            // const x_dist = (player::x > chain_point.x) ? player::x-chain_point.x : chain_point.x-player::x;
            const uint64_t x_dist = distance(player::x, chain_point.x);
            const uint64_t y_dist = distance(player::y, chain_point.y);
            //sum could overflow
            outside = x_dist*x_dist + y_dist*y_dist > uint64_t(chain_length)*chain_length;
        }
        {
            using namespace player;
            x_accel = 0;
            y_accel = 0;
            
            y_accel += gravity;

            if(outside) {
                //cast could overflow
                x_accel += (int(chain_point.x)-int(x))/100;
                y_accel += (int(chain_point.y)-int(y))/100;
            }
            
            // x_vel += x_accel;

            // x_vel = 0;
            // x_vel += 100*speed*(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D));
            // x_vel -= 100*speed*(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A));

            if(IsKeyPressed(KEY_SPACE)) {
                y_vel = -30*16;
            }
            x_vel += x_accel;
            y_vel = std::min(y_vel+y_accel, max_y_vel);
            
            // x_vel += speed*IsKeyDown(KEY_RIGHT);
            // x_vel -= speed*IsKeyDown(KEY_LEFT);
            // player::yVel += player::speed*IsKeyDown(KEY_DOWN);
            // player::yVel -= player::speed*IsKeyDown(KEY_UP);

            switch(sum_overflow(x,x_vel)) {
                using enum overflow;
                case none:
                    if(x+x_vel <= world::wdth) {
                        x += x_vel;
                        break;
                    }
                    [[fallthrough]];
                case high:
                    x = world::wdth;
                    x_vel = 0;
                    break;
                case low :
                    x = 0;
                    x_vel = 0;
                    break;
            }
            switch(sum_overflow(y,y_vel)) {
                using enum overflow;
                case none:
                    if(y+y_vel <= world::hght) {
                        y += y_vel;
                        break;
                    }
                    [[fallthrough]];
                case high:
                    y = world::hght;
                    y_vel = 0;
                    break;
                case low :
                    y = 0;
                    y_vel = 0;
                    break;
            }
    
        }
        BeginDrawing();
        constexpr uint32_t scale = 16;
        
        ClearBackground(BLACK);
        
        DrawText("Hello Raylib", 200, 200, 20, WHITE);
        DrawTexture(wabbit, 400, 200, WHITE);

        DrawText(TextFormat("x: %u\ny: %u\nxV: %d\nyV: %d", player::x, player::y, player::x_vel, player::y_vel),
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
            RED);
        EndDrawing();
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
