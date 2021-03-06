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

    /* 
....................................................................................................
.              .......................                                                             .
 7JJJJJJJJJJYPY~..                 .~?JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ7
 JPPPPPPPPG#BJ~....        .         :YPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPG#?.~YYPY!~^....        ..  .YPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP5YJJ?J5YYPPPPPJ
 ?PPPPPPP57!:.7##?^:::.            :. .~5PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPY?^:::::!5?J5PPPPJ
 ?PPPPPPGJ.  .5#P:::::             ....:JPPPPPPPPPPPPPPPPPPPPPPPPPPPPP5YJ?7!^?PJ!..!J55PPPPPPPPPPPJ
 ?PPPPPPG#?. 7##G7:::::...    ^!:...^^?GBPPPPPPPPPPPPPPPPPPPPP5YJ5Y7~:..!7?J??7:  JPPPPPPPPPPPPPPPJ
 ?PPPPPPG#P:.~#B!:^77^^:.::?Y5PP5Y55JYBGGGPPPPPPPPPPPP5Y5PPY7^.^JP5!^  :P55YP7~  .PPPPPPPPPPPPPPPPJ
 ?PPPPPPPBBY~7BP5GGPG#BPY7Y###PYJYGP.^#PYJPPPPPPPPY?~:.:5P5?~:?PPPP7^  .~~~:Y7!   ^Y5PPP5J?PPPPPPPJ
 ?PPPPPPPPB#GPBBBB5JJYGP5..!?7:...:~  J?.!PPPPPPPP5!^  ^PJ!:7PPPPPP7^  :YY55P5?!:.  ::^:::!PPPPPPPJ
 ?PPPPPPPPB#GP##Y7^.  .7G:    :.       ^.JPPPPPPPPP7^  :?^  ~JPPPPP7^  :PPPPPPP5YJ??7??J5PPPPPPPPPJ
 ?PPPPPPPPPB##B#G: ..~PYP!^^^!7J?:      ^5PPPPPPPPP7^  :7~.   ^?5PP7^  .YPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPB#BB#P^~YBB5YY55~. .?YP!. :.7PPPPPPPPPP7^  :PY7~.   ^J5???YY5PPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPB#BBBPG#G~^~~77??JYYPG^  .~PPPPPPPPPPP7^  .YPPYJ?JY55PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPPGB#G:!#BBB#BPJJ?7~?~::  .7PPPPPPPPPPY77?JY5PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPPPPB#5^YPG##BY~       ...:^?YPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPPPPPB#GY??BB#J:..    ^~.:.. :~?5PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPPPPB#GB#PBBB#7     ^?Y!:  :    :~7??JJJYYY5555PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPPPPGBP#?^?5G###J    .PG?^   :      .     .....:::^~7YPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPPPPPPGBB#5.?B:  :~7YG57^.JGY?JY: ..                      .~5PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 ?PPPPGGGG5YPY!: .5Y   ^5PG#&BPBGBBBG~ ..                        .!YPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 JGGGPJ7~: .:...  .5!  :PBGY75##5~~^.. ..                           ~YPPPPPPPPPPPPPPPPPPPPPPPPPPPPJ
 !7~:.     .       :5^  .:.  J###P.   ...                             ~YPPPPPPPPPPPPPPPPPPPPPPPPPPJ
.                   ^~  .    ~????^     .                              .~7!!!!!!!!!!!!!!!!!!!!!!!7~.
   
   
   ....................................................................................................
........................................                   .........................................
...................................    .:^!7JY5PPGPP5YJ7~^.    .....................................
.................................. :!JPB##BGPYJ??77??J5PGBBGY7:    .................................
..............................    ?&@@@&J:               .^!YG#BY~.  ...............................
............................  .:7P@@&GJ!^                     :!P&B?:  .............................
.......................... :5B#&@@&J~^^~!!!~^:.                  :J&&J. ............................
.........................   :?P@@@@&@@@@@@@@@@#GJ~.                .Y@#~ ...........................
.......................  ..:^!5@@@#577JB@&#BBB#&@@#?                 !&@? ..........................
...................... !PB#&@@&G?^ :Y#&Y~.    .:^77:                  ~&@? .........................
.......................5@@G?7~:   ?&@G:                                ?@@! ........................
......................  Y@#^    !G@@5                              ::  .#@B. .......................
....................... :&@P  .P@@@#.                              7#:  5@@! .......................
....................... :&@J  Y@@@@?                               ~@G. J@@J .......................
....................... ~@@~ :&@@@P.                                P@5 Y@@J .......................
....................... ?@@^ 7@@@?                                   Y@P#@@7 .......................
....................... P@@7 5@&@BGGGBGGGGPY?~.       :~!7???77?7!!~^7@@@@@~ .......................
....................... G@@&B@B?G&J?5&#GYY#@@@&PJJYJYB&@@@BGB@P?P@YP&#@@@@@&Y ......................
....................... J@@@!7@@@#5#B55PGB#@@@@@#?!Y@@&@@&GG5J5GP@&!::&J!#@@B. .....................
....................... :#@@P^&@@@5P#P7~^~!~7@@@~   ?#!:!?J?7~?G&@Y   B7 5@@Y ......................
........................ ~&@@&@@@@7 .       !@@&:              :BB.   : .#@B. ......................
......................... ~#@@@@@@#P7  .?JYG@@@Y             ^J5J.      5@#: .......................
.......................... :5@@@@@P^.   ^~5@G:B7       ~BY^  ..        J@&^ ........................
...........................  ~P@@@@B!.:!YB@@&Y&@P7^:^!JPG@@Y.         J@&~ .........................
.............................  G@@@@@@@@@@@@&5JYB&&#P?~. :5@&Y~    :J5@&~ ..........................
.............................. 5@@@@G?J@@@P7.  .:^~~:....  ~Y@@!   G@Y?: ...........................
.............................. ^&@@#.  #@@BPG##&######B####GB@@7  .&#   ............................
............................... 7@@@~  5@@@@@@@&5JJYY5P&&#&@@@Y   ^@G ..............................
................................ 5@@B. :&@&@@@@#?!!!!!~^...5@7    Y@Y ..............................
................................ ?@@@B^ ~Y:G@@@BB^         ~Y    !@&^ ..............................
................................ :#@@@@5^  ^@@@J^:              7@@7 ...............................
................................. ~&@@@@@#GP&@@~              ~G@&7 ................................
.................................. ^G@@@@@@@@@5           .^JB@@P^ .................................
...................................  !P&@@@@@@P        YP5#@@@P~  ..................................
.....................................  ^7P#@@@@P^     7&@@&P?:  ....................................
....................................... :~7G@@@@&5^ .P@@&5?7!: .....................................
...................................... :#@@@@@@@@@@GG@@@@@@@@5 .....................................
...................................... :@@@@@@@@@@@@@@@@@@@@@Y .....................................
...................................... .?555YYJ?Y@@@@P???JJYJ^ .....................................
........................................       :G@@@@G        ......................................
............................................  7&@@@@@@7 ............................................
..........................................  ^P@@@GY@@@P ............................................
........................................  :Y&@@@P.?@@@B. ...........................................
....................................... ^5@@@@B!  ?@@@&: ...........................................
....................................... !5PBG7. . J@@@&^ ...........................................
........................................   .   .. G@@@@~ ...........................................
........................................... ......~7J5P: ...........................................
..................................................      ............................................
....................................................................................................
   
   */

