/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"

#include "geometry.h"
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
    aabb from{0,0,50*16,50*16};
    bool paused = true;
    while (!WindowShouldClose()) {
        if(IsKeyPressed(KEY_BACKSLASH)) { paused = !paused; }
        if(!paused || IsKeyPressed(KEY_RIGHT_SHIFT)) {
            player::tick();
        }
        
        BeginDrawing();
        constexpr double scale = 16;
        
        ClearBackground(BLACK);
        
        world::draw(scale);
        
        const point to{GetMouseX()*scale, GetMouseY()*scale};
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            player::x = GetMouseX()*scale;
            player::y = GetMouseY()*scale;
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // from.x = to.x;
            // from.y = to.y;
            from.x = 3000-50*16;
            from.y = 12000-50*16;
        }
        DrawRectangle(from.x/scale, from.y/scale, from.wdth/scale, from.hght/scale, BLUE);
        DrawLine(from.x/scale, from.y/scale, to.x/scale, to.y/scale, BLUE);
        for(const auto& platform : world::platforms) {
            const auto result = aabb_to_point_over_aabb_trace(from, to, platform);
            const auto& end_point = result.end_point;
            DrawRectangle(end_point.x/scale, end_point.y/scale, from.wdth/scale, from.hght/scale, RED);
        }
        
        DrawText(TextFormat("Hello Raylib, frame time: %.4f", GetFrameTime()), 200, 200, 20, WHITE);
        DrawTexture(wabbit, 400, 200, WHITE);

        DrawText(TextFormat("x: %.0lf\ny: %.0lf\nxV: %.0lf\nyV: %.0lf", player::x, player::y, player::x_vel, player::y_vel),
            200, 250, 20, WHITE);
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
