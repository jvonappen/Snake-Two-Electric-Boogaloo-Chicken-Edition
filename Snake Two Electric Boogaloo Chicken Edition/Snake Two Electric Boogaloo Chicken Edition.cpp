// Snake Two Electric Boogaloo Chicken Edition.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//("ChickenSprite/ChickenSideWalkRight.png");

#include "raylib.h"

//#define MAX_FRAME_SPEED     15
//#define MIN_FRAME_SPEED     1
#define SQUARE_SIZE         16
#define CONGA_LINE          100

typedef struct Chicken 
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Texture2D texture;
    Color colour;
} Chicken;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color colour;
    Texture2D texture;
} Food;

const int screenWidth = 500;
const int screenHeight = 500;

static int framesCounter = 0;
static bool gameOver = false;



static Food fruit = { 0 };
static Chicken chicken[CONGA_LINE] = { 0 };
static Vector2 chickenPosition[CONGA_LINE] = { 0 };
static bool canMove = false;
static Vector2 grid = { 0 };
static int counterTail = 0;


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    InitWindow(screenWidth, screenHeight, "Snake 2: Electric Boogaloo *Chicken Edition*");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    //Texture2D chookR = LoadTexture("ChickenSprite/ChickenSideWalkRight.png");        // Texture loading
    //Texture2D chookL = LoadTexture("ChickenSprite/ChickenSideWalkLeft.png");
    //Texture2D chookU = LoadTexture("ChickenSprite/ChickenWalkBack.png");
    //Texture2D chookD = LoadTexture("ChickenSprite/ChickenWalk.png");

    //Vector2 position = { 250.0f, 250.0f };

  /*  Vector2 position0 = { 100.0f, 50.0f };
    Vector2 position1 = { 200.0f, 50.0f };
    Vector2 position2 = { 300.0f, 50.0f };
    Vector2 position3 = { 400.0f, 50.0f };*/

    //Rectangle frameRec = { 0.0f, 0.0f, (float)chookU.width / 4, (float)chookU.height };
    //int currentFrame = 0;

   // int spriteCounter = 0;
   // int spriteSpeed = 8;            // Number of spritesheet frames shown by second

    SetTargetFPS(60);               // Set game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (!gameOver)
        {


            // Update
            //----------------------------------------------------------------------------------
            grid.x = screenWidth % SQUARE_SIZE;
            grid.y = screenHeight % SQUARE_SIZE;

            framesCounter = 0;
            gameOver = false;

            counterTail = 1;
            canMove = false;

            for (int i = 0; i < CONGA_LINE; i++)
            {
                chicken[i].position = { grid.x / 2, grid.y / 2 };
                chicken[i].size = { SQUARE_SIZE, SQUARE_SIZE };
                chicken[i].speed = { SQUARE_SIZE, 0 };

                if (i == 0)
                {
                    //chicken[i].texture = chookD;
                    chicken[i].colour = DARKGREEN;
                }
                else
                {
                    //chicken[i].texture = chookR;
                    chicken[i].colour = GREEN;
                }
            }

            for (int i = 0; i < CONGA_LINE; i++)
            {
                chickenPosition[i] = { 0.0f, 0.0f };
            }

            fruit.size = { SQUARE_SIZE, SQUARE_SIZE };
            //fruit.texture = chookU;
            fruit.active = false;
            fruit.colour = PINK;

            // Sprite animation  //----------------------------------------------------------------------------------
        /*    spriteCounter++;

            if (spriteCounter >= (60 / spriteSpeed))
            {
                spriteCounter = 0;
                currentFrame++;

                if (currentFrame > 5) currentFrame = 0;

                frameRec.x = (float)currentFrame * (float)chookU.width / 4;
            }*/


            // Controls //----------------------------------------------------------------------------------
            if (IsKeyPressed(KEY_RIGHT) && (chicken[0].speed.x == 0) && canMove)
            {
                chicken[0].speed = { SQUARE_SIZE, 0 };
                canMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (chicken[0].speed.x == 0) && canMove)
            {
                chicken[0].speed = { -SQUARE_SIZE, 0 };
                canMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (chicken[0].speed.y == 0) && canMove)
            {
                chicken[0].speed = { 0, -SQUARE_SIZE };
                canMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (chicken[0].speed.y == 0) && canMove)
            {
                chicken[0].speed = { 0, SQUARE_SIZE };
                canMove = false;
            }

            // Movement //----------------------------------------------------------------------------------
            for (int i = 0; i < counterTail; i++) chickenPosition[i] = chicken[i].position;

            if ((framesCounter % 1) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        chicken[0].position.x += chicken[0].speed.x;
                        chicken[0].position.y += chicken[0].speed.y;
                        canMove = true;
                    }
                    else chicken[i].position = chickenPosition[i - 1];
                }
            }
            //----------------------------------------------------------------------------------
              // Wall behaviour
            if (((chicken[0].position.x) > (screenWidth - grid.x)) ||
                ((chicken[0].position.y) > (screenHeight - grid.y)) ||
                (chicken[0].position.x < 0) || (chicken[0].position.y < 0))
            {
                gameOver = true;
            }

            // Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((chicken[0].position.x == chicken[i].position.x) && (chicken[0].position.y == chicken[i].position.y)) gameOver = true;
            }

            // Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = { GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.y / 2 };

                for (int i = 0; i < counterTail; i++)
                {
                    while ((fruit.position.x == chicken[i].position.x) && (fruit.position.y == chicken[i].position.y))
                    {
                        fruit.position = { GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.y / 2 };
                        i = 0;
                    }
                }
            }

            // Collision
            if ((chicken[0].position.x < (fruit.position.x + fruit.size.x) && (chicken[0].position.x + chicken[0].size.x) > fruit.position.x) &&
                (chicken[0].position.y < (fruit.position.y + fruit.size.y) && (chicken[0].position.y + chicken[0].size.y) > fruit.position.y))
            {
                chicken[counterTail].position = chickenPosition[counterTail - 1];
                counterTail += 1;
                fruit.active = false;
            }

            framesCounter++;


            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);


            // Grid //----------------------------------------------------------------------------------
            for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
            {
                DrawLineV({ SQUARE_SIZE * i + grid.x / 2, grid.y / 2 }, { SQUARE_SIZE * i + grid.x / 2, screenHeight - grid.y / 2 }, LIGHTGRAY);
            }

            for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
            {
                DrawLineV({ grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, { screenWidth - grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, LIGHTGRAY);
            }

            // Draw chicken
            for (int i = 0; i < counterTail; i++) DrawRectangleV(chicken[i].position, chicken[i].size, chicken[i].colour);

            // Draw fruit to pick
            DrawRectangleV(fruit.position, fruit.size, fruit.colour);


        }
        else DrawText("Press [ENTER] to play again", GetScreenWidth() / 2, GetScreenHeight() / 2,  20, GRAY);

        /*DrawTextureRec(chookR, frameRec, position0, WHITE);
        DrawTextureRec(chookL, frameRec, position1, WHITE);
        DrawTextureRec(chookU, frameRec, position2, WHITE);
        DrawTextureRec(chookD, frameRec, position3, WHITE);*/


        //DrawTextureRec(chookD, frameRec, position, WHITE);

        //DrawText("Controls", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
    }
        //----------------------------------------------------------------------------------


     // De-Initialization
     //--------------------------------------------------------------------------------------
     //UnloadTexture(chookR);       // Texture unloading

     CloseWindow();                // Close window and OpenGL context
     //--------------------------------------------------------------------------------------

     return 0;
}