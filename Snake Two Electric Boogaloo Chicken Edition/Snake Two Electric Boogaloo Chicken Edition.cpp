// Snake Two Electric Boogaloo Chicken Edition.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//("ChickenSprite/ChickenSideWalkRight.png");

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
#define SQUARE_SIZE     16

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 500;
    const int screenHeight = 500;

    static Vector2 grid = { 0 };
    grid.x = screenWidth % SQUARE_SIZE;
    grid.y = screenHeight % SQUARE_SIZE;

    InitWindow(screenWidth, screenHeight, "Snake 2: Electric Boogaloo *Chicken Edition*");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D chookR = LoadTexture("ChickenSprite/ChickenSideWalkRight.png");        // Texture loading
    Texture2D chookL = LoadTexture("ChickenSprite/ChickenSideWalkLeft.png");
    Texture2D chookU = LoadTexture("ChickenSprite/ChickenWalkBack.png");
    Texture2D chookD = LoadTexture("ChickenSprite/ChickenWalk.png");



    Vector2 position = { 250.0f, 250.0f };

    Vector2 position0 = { 100.0f, 50.0f };
    Vector2 position1 = { 200.0f, 50.0f };
    Vector2 position2 = { 300.0f, 50.0f };
    Vector2 position3 = { 400.0f, 50.0f };

    Rectangle frameRec = { 0.0f, 0.0f, (float)chookU.width / 4, (float)chookU.height };
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

            frameRec.x = (float)currentFrame * (float)chookU.width / 4;
        }

        if (IsKeyPressed(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN))
        {
            
            DrawTextureRec(chookR, frameRec, position, WHITE); 
            position.x += 16.0f;
        }
        if (IsKeyPressed(KEY_LEFT)      && !IsKeyDown(KEY_RIGHT)&& !IsKeyDown(KEY_UP)  && !IsKeyDown(KEY_DOWN)) position.x -= 16.0f;
        if (IsKeyPressed(KEY_UP)        && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_DOWN)&& !IsKeyDown(KEY_RIGHT)) position.y -= 16.0f;
        if (IsKeyPressed(KEY_DOWN)      && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_UP)  && !IsKeyDown(KEY_RIGHT)) position.y += 16.0f;

        // Grid
        for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV( { SQUARE_SIZE* i + grid.x / 2, grid.y / 2 }, { SQUARE_SIZE* i + grid.x / 2, screenHeight - grid.y / 2 }, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV({ grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, { screenWidth - grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, LIGHTGRAY);
        }


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        /*DrawTextureRec(chookR, frameRec, position0, WHITE);  
        DrawTextureRec(chookL, frameRec, position1, WHITE);
        DrawTextureRec(chookU, frameRec, position2, WHITE);
        DrawTextureRec(chookD, frameRec, position3, WHITE);*/

        DrawTextureRec(chookD, frameRec, position, WHITE);

        
        
            // Draw grid lines
           

        //DrawText("Controls", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(chookR);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}