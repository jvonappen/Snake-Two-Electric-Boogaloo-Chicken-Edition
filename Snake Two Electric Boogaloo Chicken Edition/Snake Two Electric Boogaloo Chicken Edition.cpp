// Snake Two Electric Boogaloo Chicken Edition.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//("ChickenSprite/ChickenSideWalkRight.png");

#include "raylib.h"

//#define MAX_FRAME_SPEED     15
//#define MIN_FRAME_SPEED     1
#define SQUARE_SIZE         16
#define CONGA_LINE          256

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

typedef enum 
{
    STORAGE_POSITION_SCORE = 0,
    //STORAGE_POSITION_HISCORE = 1
} StorageData;

const int screenWidth = 500;
const int screenHeight = 500;

static int framesCounter = 0;
static bool gameOver = false;

static Food egg = { 0 };
static Chicken chicken[CONGA_LINE] = { 0 };
static Vector2 chickenPosition[CONGA_LINE] = { 0 };
static bool canMove = false;
static Vector2 grid = { 0 };
static int counterTail = 0;

int score = 0;

static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    InitWindow(screenWidth, screenHeight, "Snake 2: Electric Boogaloo *Chicken Edition*");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    // 
    //Texture2D chookL = LoadTexture("ChickenSprite/ChickenSideWalkLeft.png");
    //Texture2D chookU = LoadTexture("ChickenSprite/ChickenWalkBack.png");
    

    //Vector2 position = { 250.0f, 250.0f };

  /*  Vector2 position0 = { 100.0f, 50.0f };
    Vector2 position1 = { 200.0f, 50.0f };
    Vector2 position2 = { 300.0f, 50.0f };
    Vector2 position3 = { 400.0f, 50.0f };*/

    //Rectangle frameRec1 = { 0.0f, 0.0f, (float)chookU.width / 4, (float)chookU.height };
    
    //int currentFrame = 0;

   // int spriteCounter = 0;
   // int spriteSpeed = 8;            // Number of spritesheet frames shown by second
    
    InitGame();

    SetTargetFPS(60);               // Set game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
  //--------------------------------------------------------------------------------------
  //UnloadTexture(chookR);       // Texture unloading
    UnloadGame();
    CloseWindow();
    return 0;
}
void InitGame(void)
{
    grid.x = screenWidth % SQUARE_SIZE;
    grid.y = screenHeight % SQUARE_SIZE;

    Texture2D chookD = LoadTexture("ChickenSprite/ChickenWalk.png");
    Texture2D chookR = LoadTexture("ChickenSprite/ChickenSideWalkRight.png");
    Texture2D eggSprite = LoadTexture("ChickenSprite/Egg.png");
    Rectangle frameRec1 = { 0.0f, 0.0f, 16, 16 };

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
            chicken[i].texture = chookD; // Leader
        }
        else
        {
            chicken[i].texture = chookR;  // Followers
            //chicken[i].colour = GREEN;
            
        }
    }

    for (int i = 0; i < CONGA_LINE; i++)
    {
        chickenPosition[i] = { 0.0f, 0.0f };
    }

    egg.size = { SQUARE_SIZE, SQUARE_SIZE };
    egg.active = false;
    egg.colour = PINK;
    egg.texture = eggSprite;
}



void UpdateGame(void)
{
    if (!gameOver)
    {

        // Update
        //----------------------------------------------------------------------------------


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
            //chicken[0].texture = chookR;
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

        if ((framesCounter % 5) == 0)
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
          // Wall
        if (((chicken[0].position.x) > (screenWidth - grid.x)) ||
            ((chicken[0].position.y) > (screenHeight - grid.y)) ||
            (chicken[0].position.x < 0) || (chicken[0].position.y < 0))
        {
            gameOver = true;
        }

        // Collision with other Chickens
        for (int i = 1; i < counterTail; i++)
        {
            if ((chicken[0].position.x == chicken[i].position.x) && (chicken[0].position.y == chicken[i].position.y)) gameOver = true;
        }

        // Egg position calculation
        if (!egg.active)
        {
            egg.active = true;
            egg.position = { GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.y / 2 };

            for (int i = 0; i < counterTail; i++)
            {
                while ((egg.position.x == chicken[i].position.x) && (egg.position.y == chicken[i].position.y))
                {
                    egg.position = { GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.y / 2 };
                    i = 0;
                }
            }
        }

        // Collision
        if ((chicken[0].position.x < (egg.position.x + egg.size.x) && (chicken[0].position.x + chicken[0].size.x) > egg.position.x) &&
            (chicken[0].position.y < (egg.position.y + egg.size.y) && (chicken[0].position.y + chicken[0].size.y) > egg.position.y))
        {
            chicken[counterTail].position = chickenPosition[counterTail - 1];  // Prevents the chicken loading in the top right
            counterTail += 1;
            score += 1;
            egg.active = false;
        }
        framesCounter++;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}




    void DrawGame(void)
    {
        // Draw
            //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGREEN);

        
    
        if (!gameOver)
        {
            // Grid //----------------------------------------------------------------------------------
            for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
            {
                DrawLineV({ SQUARE_SIZE * i + grid.x / 2, grid.y / 2 }, { SQUARE_SIZE * i + grid.x / 2, screenHeight - grid.y / 2 }, LIME);
            }

            for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
            {
                DrawLineV({ grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, { screenWidth - grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, LIME);
            }

            Rectangle frameRec1 = { 0.0f, 0.0f, 16, 16 };
            // Draw chicken
            for (int i = 0; i < counterTail; i++) DrawTextureRec(chicken->texture, frameRec1, chicken[i].position, RAYWHITE);
            

            // Draw egg to hatch
            DrawTextureRec(egg.texture, frameRec1, egg.position, RAYWHITE);
           

        }

        else
        {
            DrawText("Press [ENTER] to play again!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to play again!", 15) / 2, GetScreenHeight() / 2, 15, BLACK);
            DrawText(TextFormat("SCORE: %i", score), GetScreenWidth() / 2 - MeasureText("SCORE: %i", 15) / 2, 80, 15, RED);
        }
        /*DrawTextureRec(chookR, frameRec, position0, WHITE);
        DrawTextureRec(chookL, frameRec, position1, WHITE);
        DrawTextureRec(chookU, frameRec, position2, WHITE);
        DrawTextureRec(chookD, frameRec, position3, WHITE);*/


        //DrawTextureRec(chookD, frameRec, position, WHITE);

        //DrawText("Controls", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
    }

    void UnloadGame(void)
    {
      
    }

    void UpdateDrawFrame(void)
    {
        UpdateGame();
        DrawGame();
    }
            
        //----------------------------------------------------------------------------------

