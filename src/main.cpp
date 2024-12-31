/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"

#include "common.h"
#include "world.h"
#include "player.h"

#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <limits>
#include <iostream>

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
        player::tick(chain_point, chain_length);
        
        BeginDrawing();
        constexpr double scale = 16;
        
        ClearBackground(BLACK);
        
        world::draw(scale);
        
        DrawText(TextFormat("Hello Raylib, frame time: %.4f", GetFrameTime()), 200, 200, 20, WHITE);
        DrawTexture(wabbit, 400, 200, WHITE);

        DrawText(TextFormat("x: %.0lf\ny: %.0lf\nxV: %.0lf\nyV: %.0lf", player::x, player::y, player::x_vel, player::y_vel),
            200, 250, 20, WHITE);
        DrawCircle(
            chain_point.x/scale,
            chain_point.y/scale,
            chain_length/scale,
            Color{255,255,255,uint8_t(false?60:50)});
        DrawLine(
            player::x/scale,
            player::y/scale,
            chain_point.x/scale,
            chain_point.y/scale,
            ORANGE);
        player::draw(scale);
        EndDrawing();
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
