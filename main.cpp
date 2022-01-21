
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

int score;

// function to load textures
Texture2D loadTxtr(txtr)
{
    texture = LoadTexture(txtr);
    std::cout << "[TEXTURE@ << txtr << "] - Initialized" << std::endl;
    return texture;
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

     // multi-dimensional array that represents rocks/waves possible spawning locations
    int baseMap[10][6] = {
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 5, 5, 5, 0},
        {0, 0, 5, 5, 5, 0},
        {0, 0, 5, 5, 5, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}
    }; 

     // multi-dimensional array that represents possible shrimp spawning locations
    bool baseShrimp[10][6] = {
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true},
        {true, true, true, true, true, true}
    };

    // initialize copies of each array to use for each session
    auto map = baseMap;
    auto shrimp = baseShrimp;

    // constant variables that represent the window width and height
    const int screenWidth = 600;
    const int screenHeight = 1000;

    // This initializes the OpenGL context, and creates a new window on the user's desktop
    InitWindow(screenWidth, screenHeight, "River Game");    

    // load the textures for the fish, rocks, shrimp, and waves
    Texture2D fishTxtr = loadTxtr("sprites/fish.png");                      //
    Texture2D rockTxtr = loadTxtr("sprites/rock.png");  
    Texture2D shrimpTxtr = loadTxtr("sprites/shrimp.png");  
    Texture2D wavesTxtr = loadTxtr("sprites/waves.png");

    // vector to represent the relative position of the player
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    // the width and height of the player in constant variables
    const int playerWidth = 71;
    const int playerHeight = 97;

    // Set the optimal frame rate to 60 frames per second
    SetTargetFPS(60);

    // change the key usesd to exit the program (default ESC)
    SetExitKey(KEY_F9);

    // set the global variable score to zero
    score = 0;

    // boolean that represents when the game is paused or not
    bool menuActive = false;

    // booleans that represent if the user has won/lost
    bool lose = false;
    bool win = false;

    // sets the seed for the random number generator to the current system time
    srand(time(NULL));

    // randomly assign value that represnt spawning materials to chunks on our map
    for (int row = 0; row < 10; row++) {
        for (int chunk = 0; chunk < 6; chunk++) {
            if (map[row][chunk] != 5) map[row][chunk] = rand() % 5;
        }
    }

    // rotation of our character is initialized to zero
    int rotation = 0;   
    
    // each integer in this six element array represents a possible spawning point for the wave animation
    int wavesPosition[6] = {0, 0, 0, 0, 0, 0};

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or F9 key
    {

        // check if the escape key is pressed, if so, toggle the menu
        if (IsKeyPressed(KEY_ESCAPE)) menuActive = !menuActive;

        // logic to deploy menu
        if (menuActive)
        {
            BeginDrawing();
                DrawText("PAUSED", 40, 40, 32, GRAY);
            EndDrawing();
        } else 
        {
            // detect if the player has lost
            if (lose) {
                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, Color{22, 204, 229, 55});
                    DrawText("GAME OVER", 40, 40, 50, RED);
                    DrawText("Press ENTER to continue", 40, screenHeight - 50, 22, BLACK);
                    if (IsKeyPressed(KEY_ENTER)) { playerPosition = { (float)screenWidth/2, (float)screenHeight/2 }; lose = false;
                        map = baseMap;
                        shrimp = baseShrimp;
                        for (int row = 0; row < 10; row++)
                        {
                            for (int chunk = 0; chunk < 6; chunk++)
                            {
                                if (map[row][chunk] != 5) map[row][chunk] = rand() % 5;
                            }
                        }
                        score = 0;
                    }
                EndDrawing();

            // detect if the player has won
            } else if (win)
            {
                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, Color{22, 204, 229, 55});
                    DrawText("LEVEL CLEARED", 40, 40, 50, LIME);
                    char buf[128];
                    snprintf(buf, 16, "Score: %d", score);
                    DrawText(buf, 40, 100, 45, GRAY);
                    DrawText("Press ENTER to continue", 40, screenHeight - 50, 22, BLACK);
                    if (IsKeyPressed(KEY_ENTER)) { playerPosition = { (float)screenWidth/2, (float)screenHeight/2 }; win = false;
                        map = baseMap; 
                        shrimp = baseShrimp;
                        for (int row = 0; row < 10; row++)
                        {
                            for (int chunk = 0; chunk < 6; chunk++)
                            {
                                if (map[row][chunk] != 5) map[row][chunk] = rand() % 5;
                            }
                        }
                    }
                EndDrawing();
            } else {
                
                // logic to detect player input and maneuver sprites
                if (IsKeyDown(KEY_UP)) playerPosition.y -= (float)screenHeight/192;
                if (IsKeyDown(KEY_DOWN)) playerPosition.y += (float)screenHeight/320;
                if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) { playerPosition.x -= (float)screenHeight/320; rotation = -10; }
                if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - playerWidth) { playerPosition.x += (float)screenHeight/320; rotation = 10; }

                playerPosition.y += (float)screenHeight/256;

                if (playerPosition.y + playerHeight >= screenHeight) lose = true;
                if (playerPosition.y <= 0) { score += 5; win = true; }

                // draw our textures and sprites to the screen
                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawTextureEx(fishTxtr, playerPosition, rotation, 1, RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, Color{22, 204, 229, 55});

                    // draw our waves and animate them with some randomization logic
                    for (int wave = 0; wave < 6; wave++)
                    {
                        DrawTextureEx(wavesTxtr, Vector2{(float)(wave+1)*100, (float)wavesPosition[wave]}, 0, 1.25, WHITE);
                        if (rand() % 6 == 0)
                        {
                            wavesPosition[wave] += 80;
                        }
                        if (wavesPosition[wave] >= screenHeight)
                        {
                            wavesPosition[wave] = 0;
                        }
                    }

                    // collision detection and some logic
                    for (int row = 0; row < 10; row++)
                    {
                        for (int chunk = 0; chunk < 6; chunk++)
                        {

                            if (map[row][chunk] == 1)
                            {
                                DrawTextureEx(rockTxtr, Vector2{(float)chunk*100, (float)row*100}, 0, 2, RAYWHITE);
                                if (CheckCollisionRecs(
                                    Rectangle{(float)chunk*100, (float)row*100, 88, 42},
                                    Rectangle{playerPosition.x, playerPosition.y, playerWidth, playerHeight}
                                    )) {
                                        lose = true;
                                    }
                            } else if (map[row][chunk] == 2)
                            {
                                if (shrimp[row][chunk])
                                {
                                    DrawTexture(shrimpTxtr, chunk*100, row*100, RAYWHITE);
                                    if (CheckCollisionRecs(
                                        Rectangle{(float)chunk*100, (float)row*100, 30, 30},
                                        Rectangle{playerPosition.x, playerPosition.y, playerWidth, playerHeight}
                                    )) {
                                        score += 1;
                                        shrimp[row][chunk] = false;
                                    }
                                }
                        }
                        }
                    }
                EndDrawing();
                //----------------------------------------------------------------------------------
                rotation = 0;
            }
        }
    }

    // unload all textures from RAM/VRAM
    UnloadTexture(fishTxtr);
    UnloadTexture(rockTxtr);
    UnloadTexture(shrimpTxtr);
    UnloadTexture(wavesTxtr);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}
