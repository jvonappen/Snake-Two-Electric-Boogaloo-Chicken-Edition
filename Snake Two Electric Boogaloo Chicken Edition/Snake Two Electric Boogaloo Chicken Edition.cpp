// Snake Two Electric Boogaloo Chicken Edition.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "raylib.h"

#define SQUARE_SIZE         16
#define CONGA_LINE          256

typedef struct Chicken
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Texture2D texture;
};

typedef struct Food 
{
    Vector2 position;
    Vector2 size;
    bool active;
    Texture2D texture;
};

typedef struct Enemy
{
    Vector2 position;
    Vector2 size;
    bool active;
    Texture2D texture;
};

const int screenWidth = 500;
const int screenHeight = 500;

static int framesCounter = 0;
static bool gameOver = false;

static bool gridToggle;

static Enemy fox = { 0 };
static Food egg = { 0 };
static Chicken chicken[CONGA_LINE] = { 0 };
static Vector2 chickenPosition[CONGA_LINE] = { 0 };
static bool canMove = false;
static Vector2 grid = { 0 };
static int counterTail = 0;

int score = 0;
int hiScore = 0;

static void InitGame(void);         
static void UpdateGame(void);       
static void DrawGame(void);         
static void UnloadGame(void);       
static void UpdateDrawFrame(void);  

int main(void)
{
 // Initialization //----------------------------------------------------------------------------------------
  
    
    InitWindow(screenWidth, screenHeight, "Snake 2: Electric Boogaloo *Chicken Edition*");

    int currentFrame = 0;

    InitGame();

    SetTargetFPS(60);

 // Main game loop  //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())                                  /* Detect window close button or ESC key */ 
    {
        UpdateDrawFrame();
    }
 // De-Initialization //------------------------------------------------------------------------------------

    UnloadGame();
    CloseWindow();
    return 0;
}
void InitGame(void)
{
    grid.x = screenWidth % SQUARE_SIZE;
    grid.y = screenHeight % SQUARE_SIZE;

    Texture2D chookSprite = LoadTexture("ChickenSprite/ChickenWalk.png");
    Texture2D eggSprite = LoadTexture("ChickenSprite/Egg.png");
    Texture2D foxSprite = LoadTexture("ChickenSprite/Fox.png");

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
            chicken[i].texture = chookSprite;                         /* Leader (aka snake head) */
        }
        else
        {
            chicken[i].texture = chookSprite;                         /* Followers (aka snake body) */
        }                                                             /* Originally planned to use an alternate sprite */
    }

    for (int i = 0; i < CONGA_LINE; i++)
    {
        chickenPosition[i] = { 0.0f, 0.0f };
    }

    egg.size = { SQUARE_SIZE, SQUARE_SIZE };
    egg.active = false;
    egg.texture = eggSprite;

    fox.size = { SQUARE_SIZE, SQUARE_SIZE };
    fox.active = false;
    fox.texture = foxSprite;
}

void UpdateGame(void)
{
    if (!gameOver)
    {

    // Controls //----------------------------------------------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_D) && (chicken[0].speed.x == 0) && canMove)
        {
            chicken[0].speed = { SQUARE_SIZE, 0 };
            canMove = false;
        }
        if (IsKeyPressed(KEY_A) && (chicken[0].speed.x == 0) && canMove)
        {
            chicken[0].speed = { -SQUARE_SIZE, 0 };
            canMove = false;
        }
        if (IsKeyPressed(KEY_W) && (chicken[0].speed.y == 0) && canMove)
        {
            chicken[0].speed = { 0, -SQUARE_SIZE };
            canMove = false;
        }
        if (IsKeyPressed(KEY_S) && (chicken[0].speed.y == 0) && canMove)
        {
            chicken[0].speed = { 0, SQUARE_SIZE };
            canMove = false;
        }
        if (IsKeyPressed(KEY_T))
        {
            gridToggle = !gridToggle;                                       /* Toggles the grid view */
        }
    // Movement //---------------------------------------------------------------------------------------------------------------------
        for (int i = 0; i < counterTail; i++) chickenPosition[i] = chicken[i].position;

        if ((framesCounter % 5) == 0)                                       /* Sets the speed of the 'tick' style movement */
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
    // Wall  //-----------------------------------------------------------------------------------------------------------------------
        if (((chicken[0].position.x) > (screenWidth - grid.x)) ||
            ((chicken[0].position.y) > (screenHeight - grid.y)) ||
            (chicken[0].position.x < 0) || (chicken[0].position.y < 0))
        {
            gameOver = true;
        }
    // Collision with other Chickens //-----------------------------------------------------------------------------------------------
        for (int i = 1; i < counterTail; i++)
        {
            if ((chicken[0].position.x == chicken[i].position.x) && (chicken[0].position.y == chicken[i].position.y)) gameOver = true;
        }
    // Egg position calculation //-----------------------------------------------------------------------------------------------------
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
    // Collision with Egg //------------------------------------------------------------------------------------------------------------
        if ((chicken[0].position.x < (egg.position.x + egg.size.x) && (chicken[0].position.x + chicken[0].size.x) > egg.position.x) &&
            (chicken[0].position.y < (egg.position.y + egg.size.y) && (chicken[0].position.y + chicken[0].size.y) > egg.position.y))
        {
            chicken[counterTail].position = chickenPosition[counterTail - 1];           /* Prevents the chicken loading in the top right */ 
            counterTail += 1;
            score += 1;
            hiScore += 1;
            egg.active = false;
            fox.active = false;                                                         /* Changes fox position every time an egg is hatched */
        }
    // Fox position calculation //-----------------------------------------------------------------------------------------------------
        if (!fox.active)
        {
            extraFox = !extraFox;
            fox.active = true; 
            fox.position = { GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + grid.y / 2 };
        }
    // Collision with Fox //------------------------------------------------------------------------------------------------------------
        if ((chicken[0].position.x < (fox.position.x + fox.size.x) && (chicken[0].position.x + chicken[0].size.x) > fox.position.x) &&
            (chicken[0].position.y < (fox.position.y + fox.size.y) && (chicken[0].position.y + chicken[0].size.y) > fox.position.y))
        {
            gameOver = true;
        }
        
        framesCounter++;                                                                /* Creates the "tick" style movement */
    }
    // Restart at Game Over //------------------------------------------------------------------------------------------------------------
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
            score = 0;                                                                 /* Resets game score */
        }
    }
}


    void DrawGame(void)
    {
    // Draw //-----------------------------------------------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGREEN);

        if (!gameOver)
        {
    // Grid //-----------------------------------------------------------------------------------------------------------------------
            if (gridToggle)
            {
                for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
                {
                    DrawLineV({ SQUARE_SIZE * i + grid.x / 2, grid.y / 2 }, { SQUARE_SIZE * i + grid.x / 2, screenHeight - grid.y / 2 }, LIME);
                }

                for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
                {
                    DrawLineV({ grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, { screenWidth - grid.x / 2, SQUARE_SIZE * i + grid.y / 2 }, LIME);
                }
            }
    // Characters //-----------------------------------------------------------------------------------------------------------------
            Rectangle frameRec1 = { 0.0f, 0.0f, 16, 16 };                                                                            /* Needed to load texture */
           
            for (int i = 0; i < counterTail; i++) DrawTextureRec(chicken->texture, frameRec1, chicken[i].position, RAYWHITE);        /* Draws chicken */
       
            DrawTextureRec(egg.texture, frameRec1, egg.position, RAYWHITE);                                                          /* Draws egg to hatch */
            
            DrawTextureRec(fox.texture, frameRec1, fox.position, RAYWHITE);                                                          /* Draws fox */
           
            DrawText("Controls", 10, 10, 12, GREEN);
            DrawText("Move: W . A . S . D", 10, 22, 10, GREEN);
            DrawText("Toggle Grid: T", 10, 32, 10, GREEN);
        }
    // Game Over Screen //----------------------------------------------------------------------------------------------------------
        else
        {
            DrawText("Press [ENTER] to play again!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to play again!", 15) / 2, GetScreenHeight() / 2, 15, GREEN);
            DrawText(TextFormat("SCORE: %i", score), GetScreenWidth() / 2 - MeasureText("SCORE: %i", 15) / 2, 80, 15, GREEN);
            DrawText(TextFormat("HIGH SCORE: %i", hiScore), GetScreenWidth() / 2 - MeasureText("HIGH SCORE: %i", 15) / 2, 100, 15, GREEN);
        }

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


