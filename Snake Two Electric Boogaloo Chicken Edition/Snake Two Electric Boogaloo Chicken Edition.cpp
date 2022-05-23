// Snake Two Electric Boogaloo Chicken Edition.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//("ChickenSprite/ChickenSideWalkRight.png");

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 500;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "Snake 2: Electric Boogaloo *Chicken Edition*");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D chook = LoadTexture("ChickenSprite/ChickenSideWalkRight.png");        // Texture loading

    Vector2 position = { 250.0f, 250.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)chook.width / 4, (float)chook.height };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;            // Number of spritesheet frames shown by second

    SetTargetFPS(60);               // Set game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5) currentFrame = 0;

            frameRec.x = (float)currentFrame * (float)chook.width / 4;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(chook, frameRec, position, WHITE);  // Draw part of the texture

        //DrawText("Controls", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(chook);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}