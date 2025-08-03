#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <raylib.h>

// 16x9 resolution
#define FACTOR 120
#define WIDTH 16*FACTOR
#define HEIGHT 9*FACTOR

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Portable raylib");
    SetTargetFPS(60);
    printf("Raylib %s\n", RAYLIB_VERSION);

    Vector2 pos = {0}; 
    pos.x = WIDTH/2.f;
    pos.y = HEIGHT/2.f;

    Color BG_COLOR = (Color){
        .r = 18,
        .g = 18,
        .b = 18,
        .a = 255,
    };

    bool auto_move = true;
    char dir_x = 1;
    char dir_y = 1;
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        // Logic Loop
        if (auto_move) {
            if (pos.x >= WIDTH-25) {
                dir_x *= -1;
            }
            if (pos.x <= 25) {
                dir_x *= -1;
            }
            if (pos.y >= HEIGHT-25) {
                dir_y *= -1;
            }
            if (pos.y <= 25) {
                dir_y *= -1;
            }
            pos.x += 500*dt*dir_x;
            pos.y += 500*dt*dir_y;
        }
        if (IsKeyDown(KEY_W)) {
            pos.y -= 1000*dt;
            auto_move = false;
        }
        if (IsKeyDown(KEY_A)) {
            pos.x -= 1000*dt;
            auto_move = false;
        }
        if (IsKeyDown(KEY_S)) {
            pos.y += 1000*dt;
            auto_move = false;
        }
        if (IsKeyDown(KEY_D)) {
            pos.x += 1000*dt;
            auto_move = false;
        }
        // Draw Loop
        BeginDrawing();
        ClearBackground(BG_COLOR);
        DrawCircleV(pos, 50, YELLOW);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
