#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TILES 8
#define TILE_WIDTH 100
#define TILE_HEIGHT 150

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 60

typedef struct {
    Rectangle rec;
    Color color;
    bool active;
    char letter;
} Tile;
typedef struct {
    int fadedHighScore;
    int fadedTotalScore;
    int fadedGamesPlayed;
    int linkHighScore;
    int linkTotalScore;
    int linkGamesPlayed;
    int skyHighHighScore;
    int skyHighTotalScore;
    int skyHighGamesPlayed;
} SongHighScores;
enum GameState {
    FRONT,
    TITLE,
    SONG_SELECTION,
    HIGH_SCORE,
    LEVEL,
    PLAYING,
    GAME_OVER
};

void InitGame();
void UpdateGame();
void DrawGame();
void DrawTile(Tile tile);
bool IsTileHit(Tile tile);
void CheckGameOver();
int highScore(int song);
void ResetUsedLetters();
void DrawButton(const char* text, Rectangle position, bool hovered);
double averageScore(int song);
int gamesPlayed(int song);

Tile tiles[MAX_TILES];
Texture2D backgroundTexture;
Font gameFont;
Music Selected;
Music Faded;
Music Link;
Music Sky_High;
Rectangle FadedB;
Rectangle LinkB;
Rectangle Sky_HighB;
Rectangle start = { GetScreenWidth() / 2 - 300 , GetScreenHeight() / 2 - 300 , BUTTON_WIDTH , BUTTON_HEIGHT };
Rectangle high_score = { GetScreenWidth() / 2 - 300 ,GetScreenHeight() / 2 - 350 ,  BUTTON_WIDTH , BUTTON_HEIGHT };
Rectangle exit = { GetScreenWidth() / 2 - 300 ,GetScreenHeight() / 2 - 400 ,  BUTTON_WIDTH , BUTTON_HEIGHT };
Rectangle one, two, three, four, five, six, seven, eight, nine, ten;
SongHighScores songHighScores = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
Color button = { 0,0,70,255 };
Color buttonNormalColor = { 0, 0, 70, 255 };
Color buttonHoverColor = { 30, 30, 100, 255 };

Rectangle back1,back2,back3,back4,back5,back6;

double startTime = 0.0;
double pitchModifier = 1.0f;
double tileSpeedMultiplier = 2.0f;
double increaseInterval = 60.0;
float baseTileSpeed = 1.0f;
bool increaseApplied = false;
int score = 0;
bool gameOver = false;
static int highscore;
double lastIncreaseTime = 0.0;
static char usedLetters[26] = { 0 };

int screenWidth = 800;
int screenHeight = 1200;
float x = 0, y = 0;
int monitor = GetCurrentMonitor();
int Songs[3] = { 0 };

int beat[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
, 8, 8, 8, 8
};

int main() {

    screenWidth = 800;
    screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "Piano Tiles Game");
    GameState gameState = FRONT;
    gameFont = LoadFontEx("C:\\Windows\\Fonts\\candara.ttf", 30, 0, 250);
    InitAudioDevice();

    Faded = LoadMusicStream("Songs\\faded.mp3");
    Link = LoadMusicStream("Songs\\link.mp3");
    Sky_High = LoadMusicStream("Songs\\sky_high.mp3");

    SetTargetFPS(60);

    gameState = FRONT;


    Image background = LoadImage("Backgrounds\\background2.png");
    ImageResize(&background, GetScreenWidth(), GetScreenHeight());
    backgroundTexture = LoadTextureFromImage(background);
    UnloadImage(background);
    Image background2 = LoadImage("Backgrounds\\background1.png");
    SetWindowIcon(background2);
    ImageResize(&background2, GetScreenWidth(), GetScreenHeight());
    Texture2D backgrounTexture2 = LoadTextureFromImage(background2);
    UnloadImage(background2);
    while (!WindowShouldClose()) {
        switch (gameState) {
        case FRONT:
            BeginDrawing();
            DrawTexture(backgrounTexture2, 0, 0, WHITE);

            if (IsKeyPressed(KEY_SPACE)) gameState = TITLE;
            EndDrawing();
            break;
        case TITLE:

            SetWindowState(FLAG_WINDOW_MAXIMIZED);
            SetWindowState(FLAG_WINDOW_RESIZABLE);
            SetWindowMinSize(800, 600);
            Image background = LoadImage("Backgrounds\\background2.png");
            ImageResize(&background, GetScreenWidth(), GetScreenHeight());
            backgroundTexture = LoadTextureFromImage(background);
            UnloadImage(background);
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(backgroundTexture, 0, 0, WHITE);
            back1 = { (float)GetScreenWidth() / 2  - 250, (float)GetScreenHeight() / 2 - 440  ,500,130 };
            DrawRectangleRounded(back1, 0.5, 10, { 100, 100, 200, 255 });
            back2 = { (float)GetScreenWidth() / 2 - 340, (float)GetScreenHeight() / 2 - 240  ,700,300 };
            DrawRectangleRounded(back2, 0.5, 10, { 100, 100, 200, 255 });
            DrawTextEx(gameFont, "Piano Tiles Game", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Piano Tiles Game", 50, 0).x / 2, (float)GetScreenHeight() / 2 - 400 }, 50, 0, WHITE);
            DrawTextEx(gameFont, "Rules:", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Rules:", 40, 0).x / 2, (float)GetScreenHeight() / 2 - 200 }, 40, 0, WHITE);
            DrawTextEx(gameFont, "Press 'A' to 'Z' keys to hit corresponding tiles", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Press 'A' to 'Z' keys to hit corresponding tiles", 30, 0).x / 2, (float)GetScreenHeight() / 2 - 107 }, 30, 0, WHITE);
            DrawTextEx(gameFont, "Hit as many tiles as you can!", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Hit as many tiles as you can!", 30, 0).x / 2, (float)GetScreenHeight() / 2 - 70 }, 30, 0, WHITE);
            DrawTextEx(gameFont, "Left Arrow brings you back here", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Left Arrow brings you back here", 30, 0).x / 2, (float)GetScreenHeight() / 2 - 33 }, 30, 0, WHITE);

            

            start = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - BUTTON_HEIGHT / 2 + 100, BUTTON_WIDTH , BUTTON_HEIGHT };
            high_score = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 ,(float)GetScreenHeight() / 2 - BUTTON_HEIGHT / 2 + 170 ,  BUTTON_WIDTH , BUTTON_HEIGHT };
            exit = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 ,(float)GetScreenHeight() / 2 - BUTTON_HEIGHT / 2 + 240,  BUTTON_WIDTH , BUTTON_HEIGHT };

            DrawButton("Start", start, CheckCollisionPointRec(GetMousePosition(), start));
            DrawButton("High Scores", high_score, CheckCollisionPointRec(GetMousePosition(), high_score));
            DrawButton("Exit", exit, CheckCollisionPointRec(GetMousePosition(), exit));

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                printf("Mouse Position: (%f, %f)\n", mousePos.x, mousePos.y);

                if (CheckCollisionPointRec(mousePos, start)) {
                    printf("Start button clicked!\n");

                    gameState = SONG_SELECTION;
                    startTime = GetTime();
                }
                else if (CheckCollisionPointRec(mousePos, high_score)) {
                    printf("High Score button clicked!\n");

                    gameState = HIGH_SCORE;

                }
                else if (CheckCollisionPointRec(mousePos, exit)) {
                    printf("Exit button clicked!\n");
                    CloseWindow();
                    return 0;
                }
            }

            if (IsKeyPressed(KEY_SPACE)) {
                InitGame();
                gameState = SONG_SELECTION;
                startTime = GetTime();

            }

            EndDrawing();
            break;
        case SONG_SELECTION:
            BeginDrawing();
            ClearBackground(RAYWHITE);


            DrawTexture(backgroundTexture, 0, 0, WHITE);

            back3 = { (float)GetScreenWidth() / 2 - 250, (float)GetScreenHeight() / 2 - 440  ,500,130 };
            DrawRectangleRounded(back3, 0.5, 10, { 100, 100, 200, 255 });
            DrawTextEx(gameFont, "Select a Song", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Select a Song", 50, 0).x / 2,  (float)GetScreenHeight() / 2 - 400 }, 50, 0, WHITE);

            FadedB = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 100,BUTTON_WIDTH,BUTTON_HEIGHT };
            LinkB = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 30,BUTTON_WIDTH,BUTTON_HEIGHT };
            Sky_HighB = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 40,BUTTON_WIDTH,BUTTON_HEIGHT };

            DrawButton("Fade", FadedB, CheckCollisionPointRec(GetMousePosition(), FadedB));
            DrawButton("Sky High", Sky_HighB, CheckCollisionPointRec(GetMousePosition(), Sky_HighB));
            DrawButton("Link", LinkB, CheckCollisionPointRec(GetMousePosition(), LinkB));

            Vector2 pos;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pos = { (float)GetMouseX(),(float)GetMouseY() };
            if (CheckCollisionPointRec(pos, FadedB)) {
                Songs[0] = 1;
                gameState = LEVEL;
            }
            if (CheckCollisionPointRec(pos, Sky_HighB)) {
                Songs[2] = 1;
                gameState = LEVEL;  
            }
            if (CheckCollisionPointRec(pos, LinkB)) {
                Songs[1] = 1;
                gameState = LEVEL;
            }
            if (IsKeyPressed(KEY_LEFT))
                gameState = TITLE;
            EndDrawing();
            break;

        case LEVEL:
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            back3 = { (float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 - 510  ,400,68 };
            DrawRectangleRounded(back3, 0.5, 10, { 100, 100, 200, 255 });
            DrawTextEx(gameFont, "Select Difficulty", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Select Difficulty", 50, 0).x / 2,
                                             (float)GetScreenHeight() / 2 - 500 }, 50, 0, WHITE);
            one = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2, (float)GetScreenHeight() / 2 - 400 , BUTTON_WIDTH , BUTTON_HEIGHT };
            two = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - 330, BUTTON_WIDTH , BUTTON_HEIGHT };
            three = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - 260 , BUTTON_WIDTH , BUTTON_HEIGHT };
            four = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - 190 , BUTTON_WIDTH , BUTTON_HEIGHT };
            five = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - 120, BUTTON_WIDTH , BUTTON_HEIGHT };
            six = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 - 50 , BUTTON_WIDTH , BUTTON_HEIGHT };
            seven = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 + 20 , BUTTON_WIDTH , BUTTON_HEIGHT };
            eight = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 + 90, BUTTON_WIDTH , BUTTON_HEIGHT };
            nine = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 + 160 , BUTTON_WIDTH , BUTTON_HEIGHT };
            ten = { (float)GetScreenWidth() / 2 - BUTTON_WIDTH / 2 , (float)GetScreenHeight() / 2 + 230, BUTTON_WIDTH , BUTTON_HEIGHT };
            DrawButton("1", one, CheckCollisionPointRec(GetMousePosition(), one));
            DrawButton("2", two, CheckCollisionPointRec(GetMousePosition(), two));
            DrawButton("3", three, CheckCollisionPointRec(GetMousePosition(), three));
            DrawButton("4", four, CheckCollisionPointRec(GetMousePosition(), four));
            DrawButton("5", five, CheckCollisionPointRec(GetMousePosition(), five));
            DrawButton("6", six, CheckCollisionPointRec(GetMousePosition(), six));
            DrawButton("7", seven, CheckCollisionPointRec(GetMousePosition(), seven));
            DrawButton("8", eight, CheckCollisionPointRec(GetMousePosition(), eight));
            DrawButton("9", nine, CheckCollisionPointRec(GetMousePosition(), nine));
            DrawButton("10", ten, CheckCollisionPointRec(GetMousePosition(), ten));

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(GetMousePosition(), one)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 0.5f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), two)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 1.0f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), three)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 1.5f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), four)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 2.0f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), five)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 2.5f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), six)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 3.0f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), seven)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 3.5f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), eight)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 4.0f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), nine)) {

                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 4.5f;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), ten)) {
                    if (Songs[0] == 1) PlayMusicStream(Faded);
                    else if (Songs[1] == 1) PlayMusicStream(Link);
                    else if (Songs[2] == 1) PlayMusicStream(Sky_High);
                    gameState = PLAYING;
                    startTime = GetTime();
                    InitGame();
                    baseTileSpeed = 5.0f;

                }
            }

            if (IsKeyPressed(KEY_LEFT))
                gameState = TITLE;
            EndDrawing();
            break;
        case HIGH_SCORE:
            ClearBackground(RAYWHITE);
            BeginDrawing();
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Title
            back5 = { (float)GetScreenWidth() / 2 - 160, (float)GetScreenHeight() / 2 - 465  ,300,70 };
            DrawRectangleRounded(back5, 0.5, 10, { 100, 100, 200, 255 });
            DrawTextEx(gameFont, "Scores", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Scores", 50, 0).x / 2,
                                             (float)GetScreenHeight() / 2 - 450 }, 50, 0, WHITE);

            // Song 1: Faded
            back6 = { (float)GetScreenWidth() / 2 - 900, (float)GetScreenHeight() / 2 - 315  ,1800,450 };
            DrawRectangleRounded(back6, 0.5, 10, { 100, 100, 200, 255 });
            DrawTextEx(gameFont, "Faded", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Faded", 40, 0).x / 2 - 650,
                                            (float)GetScreenHeight() / 2 - 250 }, 40, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(0)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2 - 650,
                  static_cast<float>(GetScreenHeight()) / 2 - 160 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(0)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 00000", 30, 0).x / 2 - 650,
                  static_cast<float>(GetScreenHeight()) / 2 - 70 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Games Played: %d", static_cast<int>(gamesPlayed(0))),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Games Played: 000", 30, 0).x / 2 - 650,
                  static_cast<float>(GetScreenHeight()) / 2 + 20 },
                30, 0, WHITE);

            // Song 2: Link
            DrawTextEx(gameFont, "Link", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Link", 40, 0).x / 2,
                                           (float)GetScreenHeight() / 2 -250 }, 40, 0, WHITE);

            DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(1)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2,
                  static_cast<float>(GetScreenHeight()) / 2 - 160 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(1)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 000", 30, 0).x / 2,
                  static_cast<float>(GetScreenHeight()) / 2 -70 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Games Played: %d", static_cast<int>(gamesPlayed(1))),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Games Played: 000", 30, 0).x / 2,
                  static_cast<float>(GetScreenHeight()) / 2 +20 },
                30, 0, WHITE);

            // Song 3: Sky High
            DrawTextEx(gameFont, "Sky High", { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont, "Sky High", 40, 0).x / 2 + 650,
                                               (float)GetScreenHeight() / 2 -250 }, 40, 0, WHITE);

            DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(2)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2 + 650,
                  static_cast<float>(GetScreenHeight()) / 2 -160 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(2)),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 000", 30, 0).x / 2 + 650,
                  static_cast<float>(GetScreenHeight()) / 2 -70 },
                30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Games Played: %d", static_cast<int>(gamesPlayed(2))),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Games Played: 000", 30, 0).x / 2 + 650,
                  static_cast<float>(GetScreenHeight()) / 2 + 20 },
                30, 0, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_LEFT))
                gameState = TITLE;
            break;
        case PLAYING:

            if (Songs[0] == 1)
            {
                UpdateMusicStream(Faded);
                SetMusicPitch(Faded, pitchModifier);
            }
            else if (Songs[2] == 1)
            {
                UpdateMusicStream(Sky_High);
                SetMusicPitch(Sky_High, pitchModifier);
            }
            else if (Songs[1] == 1)
            {
                UpdateMusicStream(Link);
                SetMusicPitch(Link, pitchModifier);
            }
            UpdateGame();
            DrawGame();

            if (gameOver) {
                gameState = GAME_OVER;
                if (Songs[0] == 1)
                {
                    StopMusicStream(Faded);

                }
                else    if (Songs[1] == 1)
                {
                    StopMusicStream(Sky_High);

                }
                else if (Songs[2] == 1)
                {
                    StopMusicStream(Link);

                }
            }
            break;

        case GAME_OVER:
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(backgroundTexture, 0, 0, WHITE);

            back4 = { (float)GetScreenWidth() / 2 - 250, (float)GetScreenHeight() / 2 -315  ,500,190 };
            DrawRectangleRounded(back4, 0.5, 10, { 100, 100, 200, 255 });

            ClearBackground(RAYWHITE);
            if (Songs[0] == 1)
                DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(0)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 300 },
                    30, 0, WHITE);
            if (Songs[1] == 1)
                DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(1)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 300 },
                    30, 0, WHITE);
            if (Songs[2] == 1)
                DrawTextEx(gameFont, TextFormat("High Score: %d", highScore(2)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "High Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 300 },
                    30, 0, WHITE);
            if (Songs[0] == 1)
                DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(0)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 230},
                    30, 0, WHITE);
            if (Songs[1] == 1)
                DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(1)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 230 },
                    30, 0, WHITE);
            if (Songs[2] == 1)
                DrawTextEx(gameFont, TextFormat("Average Score: %.2lf", averageScore(2)),
                    { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Average Score: 000", 30, 0).x / 2,
                      static_cast<float>(GetScreenHeight()) / 2 - 230 },
                    30, 0, WHITE);
            DrawTextEx(gameFont, TextFormat("Score: %d", score),
                { static_cast<float>(GetScreenWidth()) / 2 - MeasureTextEx(gameFont, "Score: 000", 30, 0).x / 2,
                  static_cast<float>(GetScreenHeight()) / 2 - 160 },
                30, 0, WHITE);

            Rectangle restartButtonRect = { static_cast<float>(GetScreenWidth()) / 2 - 100, static_cast<float>(GetScreenHeight()) / 2 + 120, 200, 50 };
            DrawButton("Restart", restartButtonRect, CheckCollisionPointRec(GetMousePosition(), restartButtonRect));

            Rectangle quitButtonRect = { static_cast<float>(GetScreenWidth()) / 2 - 100, static_cast<float>(GetScreenHeight()) / 2 + 190, 200, 50 };
            DrawButton("Quit", quitButtonRect, CheckCollisionPointRec(GetMousePosition(), quitButtonRect));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(GetMousePosition(), restartButtonRect)) {


                    InitGame();
                    gameState = PLAYING;
                    gameOver = false;
                    score = 0;
                    startTime = GetTime();


                    if (Songs[0] == 1)
                    {
                        PlayMusicStream(Faded);

                    }
                    else    if (Songs[2] == 1)
                    {
                        PlayMusicStream(Sky_High);

                    }
                    else if (Songs[1] == 1)
                    {
                        PlayMusicStream(Link);

                    }

                }

                else if (CheckCollisionPointRec(GetMousePosition(), quitButtonRect)) {
                    gameOver = false;
                    score = 0;
                    gameState = TITLE;
                    for (int l=0;l<3;l++)
                    Songs[l] = 0;

                }
            }
            EndDrawing();
            break;
        }


    }
    CloseWindow();

    return 0;

}
void InitGame() {

    Image background = LoadImage("Backgrounds\\background2.png");
    ImageResize(&background, GetScreenWidth(), GetScreenHeight());

    backgroundTexture = LoadTextureFromImage(background);
    UnloadImage(background);

    ResetUsedLetters();
    for (int i = 0; i < MAX_TILES; i++) {
        tiles[i].rec = { (float)(i * 100) + ((GetScreenWidth() / 2 - 400)), 0, TILE_WIDTH, TILE_HEIGHT };
        tiles[i].active = false;
        tiles[i].color = { 0,0,70,255 };

    }
}
void UpdateGame() {
    double currentTime = GetTime() - startTime;
    if (currentTime - lastIncreaseTime >= increaseInterval && !increaseApplied) {
        pitchModifier += 0.3f;
        tileSpeedMultiplier += 2.0f;
        increaseApplied = true;
        lastIncreaseTime = currentTime;
    }
    
    if (currentTime >= increaseInterval && !increaseApplied) {
        pitchModifier += 0.3f;
        tileSpeedMultiplier += 1.0f;
        increaseApplied = true;
        startTime = GetTime();
    }
    
    if (!gameOver) {

        for (int i = 0; i < MAX_TILES; i++) {
            if (tiles[i].active && IsTileHit(tiles[i])) {
                score += tileSpeedMultiplier * baseTileSpeed;
                usedLetters[tiles[i].letter - 'A'] = 0;
                tiles[i].active = false;
                printf("Tile in column %d hit!\n", i);
            }
        }
    }
    if (!gameOver) {

        for (int i = 0; i < MAX_TILES; i++) {
            if (tiles[i].active) {
                tiles[i].rec.y += baseTileSpeed * tileSpeedMultiplier;


                if (tiles[i].rec.y + TILE_HEIGHT >= GetScreenHeight()) {
                    gameOver = true;
                    increaseApplied = false;
                    pitchModifier = 1.0f;
                    tileSpeedMultiplier = 2.0f;
                    break;
                }
            }
            else {
                for (int j = 0; j < 180; j++)
                    if (GetRandomValue(0, 75000) < beat[j]) {
                        tiles[i].active = true;
                        tiles[i].rec.y = 0;

                        do {
                            tiles[i].letter = 'A' + GetRandomValue(0, 25);
                        } while (tiles[i].letter == 'R' || tiles[i].letter == 'H' || usedLetters[tiles[i].letter - 'A']);

                        usedLetters[tiles[i].letter - 'A'] = 1;
                    }
            }
        }

        CheckGameOver();
    }
}
void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);


    DrawTexture(backgroundTexture, 0, 0, WHITE);
    for (int i = 0; i <= 8; i++)
        DrawLine((i * 100) + (GetScreenWidth() / 2 - 400), 0, (i * 100) + (GetScreenWidth() / 2 - 400), GetScreenHeight(), WHITE);

    for (int i = 0; i < MAX_TILES; i++) {
        if (tiles[i].active) {
            DrawTile(tiles[i]);
        }
    }
    DrawTextEx(gameFont, TextFormat("Score: %d", score), { (float)GetScreenWidth() / 2 - MeasureTextEx(gameFont,
        "Score: 000", 50, 0).x / 2, (float)GetScreenHeight() / 2 - 500 }, 50, 0, WHITE);

    EndDrawing();
}
bool IsTileHit(Tile tile) {
    int keyCode = KEY_A + (tile.letter - 'A');


    if (IsKeyDown(keyCode)) {
        return true;
    }

    return false;
}
void DrawTile(Tile tile) {
    // Adjust brightness to simulate shading
    Color shadedColor = Color{
    (unsigned char)(tile.color.r * 0.8f),
    (unsigned char)(tile.color.g * 0.8f),
    (unsigned char)(tile.color.b * 0.8f),
    tile.color.a
    };

    DrawRectangleRounded(tile.rec, 0.5, 10, shadedColor);
    DrawTextEx(gameFont, TextFormat("%c", tile.letter),
        { tile.rec.x + TILE_WIDTH / 2 - MeasureTextEx(gameFont, TextFormat("%c", tile.letter), 30, 0).x / 2,
        tile.rec.y + TILE_HEIGHT / 2 - 10 }, 30, 0, WHITE);


}
void CheckGameOver() {
    for (int i = 0; i < MAX_TILES; i++) {
        if (tiles[i].active && tiles[i].rec.y + TILE_HEIGHT >= GetScreenHeight()) {
            gameOver = true;
            if (Songs[0] == 1)
            {
                StopMusicStream(Faded);
                FILE* file;
                errno_t err;

                // Load the high scores and game statistics from the file
                err = fopen_s(&file, "highscores.txt", "r");
                if (err == 0 && file != NULL) {
                    fscanf_s(file, "%d %d %d %d %d %d %d %d %d", &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed, &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed, &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }
                songHighScores.fadedGamesPlayed += 1;
                songHighScores.fadedTotalScore += score;

                err = fopen_s(&file, "highscores.txt", "w");
                if (err == 0 && file != NULL) {
                    fprintf(file, "%d %d %d %d %d %d %d %d %d", songHighScores.fadedHighScore, songHighScores.fadedTotalScore, songHighScores.fadedGamesPlayed, songHighScores.linkHighScore, songHighScores.linkTotalScore, songHighScores.linkGamesPlayed, songHighScores.skyHighHighScore, songHighScores.skyHighTotalScore, songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }


            }
            else    if (Songs[2] == 1)
            {
                StopMusicStream(Sky_High);
                FILE* file;
                errno_t err;

                // Load the high scores and game statistics from the file
                err = fopen_s(&file, "highscores.txt", "r");
                if (err == 0 && file != NULL) {
                    fscanf_s(file, "%d %d %d %d %d %d %d %d %d", &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed, &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed, &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }
                songHighScores.skyHighGamesPlayed += 1;
                songHighScores.skyHighTotalScore += score;

                err = fopen_s(&file, "highscores.txt", "w");
                if (err == 0 && file != NULL) {
                    fprintf(file, "%d %d %d %d %d %d %d %d %d", songHighScores.fadedHighScore, songHighScores.fadedTotalScore, songHighScores.fadedGamesPlayed, songHighScores.linkHighScore, songHighScores.linkTotalScore, songHighScores.linkGamesPlayed, songHighScores.skyHighHighScore, songHighScores.skyHighTotalScore, songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }

            }
            else if (Songs[1] == 1)
            {
                StopMusicStream(Link);
                FILE* file;
                errno_t err;

                // Load the high scores and game statistics from the file
                err = fopen_s(&file, "highscores.txt", "r");
                if (err == 0 && file != NULL) {
                    fscanf_s(file, "%d %d %d %d %d %d %d %d %d", &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed, &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed, &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }
                songHighScores.linkGamesPlayed += 1;
                songHighScores.linkTotalScore += score;

                err = fopen_s(&file, "highscores.txt", "w");
                if (err == 0 && file != NULL) {
                    fprintf(file, "%d %d %d %d %d %d %d %d %d", songHighScores.fadedHighScore, songHighScores.fadedTotalScore, songHighScores.fadedGamesPlayed, songHighScores.linkHighScore, songHighScores.linkTotalScore, songHighScores.linkGamesPlayed, songHighScores.skyHighHighScore, songHighScores.skyHighTotalScore, songHighScores.skyHighGamesPlayed);
                    fclose(file);
                }

            }

            break;
        }
    }
}
void ResetUsedLetters() {
    for (int i = 0; i < 26; i++) {
        usedLetters[i] = 0;
    }
}
int highScore(int song) {
    FILE* file;
    errno_t err;

    // Load the high scores and game statistics from the file
    err = fopen_s(&file, "highscores.txt", "r");
    if (err == 0 && file != NULL) {
        fscanf_s(file, "%d %d %d %d %d %d %d %d %d", &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed, &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed, &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
        fclose(file);
    }

    int currentHighScore = 0;

    // Save the high score and update the game statistics
    switch (song) {
    case 0: // Faded
        if (score > songHighScores.fadedHighScore) {
            songHighScores.fadedHighScore = score;
        }
        currentHighScore = songHighScores.fadedHighScore;
        break;
    case 1: // Link
        if (score > songHighScores.linkHighScore) {
            songHighScores.linkHighScore = score;
        }
        currentHighScore = songHighScores.linkHighScore;
        break;
    case 2: // Sky High
        if (score > songHighScores.skyHighHighScore) {
            songHighScores.skyHighHighScore = score;
        }
        currentHighScore = songHighScores.skyHighHighScore;
        break;
    }

    // Save the updated high scores and game statistics to the file
    err = fopen_s(&file, "highscores.txt", "w");
    if (err == 0 && file != NULL) {
        fprintf(file, "%d %d %d %d %d %d %d %d %d", songHighScores.fadedHighScore, songHighScores.fadedTotalScore, songHighScores.fadedGamesPlayed, songHighScores.linkHighScore, songHighScores.linkTotalScore, songHighScores.linkGamesPlayed, songHighScores.skyHighHighScore, songHighScores.skyHighTotalScore, songHighScores.skyHighGamesPlayed);
        fclose(file);
    }

    return currentHighScore;
}
double averageScore(int song) {
    FILE* file;
    errno_t err;

    // Load the high scores and game statistics from the file
    err = fopen_s(&file, "highscores.txt", "r");
    if (err == 0 && file != NULL) {
        fscanf_s(file, "%d %d %d %d %d %d %d %d %d",
            &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed,
            &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed,
            &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
        fclose(file);
    }

    int totalScore = 0;
    int gamesPlayed = 0;
    double currentAverage = 0.0;

    // Calculate the total score and games played based on the selected song
    switch (song) {
    case 0: // Faded
        totalScore = songHighScores.fadedTotalScore;
        gamesPlayed = songHighScores.fadedGamesPlayed;
        break;
    case 2: // Link
        totalScore = songHighScores.skyHighTotalScore;
        gamesPlayed = songHighScores.skyHighGamesPlayed;
        break;
    case 1: // Sky High
        totalScore = songHighScores.linkTotalScore;
        gamesPlayed = songHighScores.linkGamesPlayed;
        break;
    }

    // Calculate the average score and handle division by zero
    if (gamesPlayed > 0) {
        currentAverage = (double)totalScore / gamesPlayed;
    }
    else {
        currentAverage = 0.0; // Set average to 0 if no games have been played
    }

    // Save the updated high scores and game statistics to the file
    err = fopen_s(&file, "highscores.txt", "w");
    if (err == 0 && file != NULL) {
        fprintf(file, "%d %d %d %d %d %d %d %d %d",
            songHighScores.fadedHighScore, songHighScores.fadedTotalScore, songHighScores.fadedGamesPlayed,
            songHighScores.linkHighScore, songHighScores.linkTotalScore, songHighScores.linkGamesPlayed,
            songHighScores.skyHighHighScore, songHighScores.skyHighTotalScore, songHighScores.skyHighGamesPlayed);
        fclose(file);
    }

    return currentAverage;
}
void DrawButton(const char* text, Rectangle position, bool hovered) {
    Rectangle buttonRect = { position.x, position.y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Define colors based on a consistent theme
    Color buttonNormalColor = { 100, 100, 200, 255 };
    Color buttonHoverColor = { 130, 130, 230, 255 };
    Color buttonTextColor = WHITE;
    Color buttonTextHoverColor = { 255, 255, 0, 255 }; // yellow color on hover for text

    // Apply a shadow effect for depth
    Rectangle shadowRect = { buttonRect.x + 5, buttonRect.y + 5, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Draw shadow first
    DrawRectangleRounded(shadowRect, 0.5, 10, Fade(BLACK, 0.3f));

    // Draw button with appropriate color based on hover state
    DrawRectangleRounded(buttonRect, 0.5, 10, hovered ? buttonHoverColor : buttonNormalColor);

    // Draw button text with appropriate color based on hover state
    int fontSize = 20;
    Vector2 textSize = MeasureTextEx(gameFont, text, fontSize, 1);
    DrawTextEx(gameFont, text,
        { buttonRect.x + (BUTTON_WIDTH - textSize.x) / 2,
          buttonRect.y + (BUTTON_HEIGHT - textSize.y) / 2 },
        fontSize, 1, hovered ? buttonTextHoverColor : buttonTextColor);

}
int gamesPlayed(int song) {

    FILE* file;
    errno_t err;

    // Load the high scores and game statistics from the file
    err = fopen_s(&file, "highscores.txt", "r");
    if (err == 0 && file != NULL) {
        fscanf_s(file, "%d %d %d %d %d %d %d %d %d",
            &songHighScores.fadedHighScore, &songHighScores.fadedTotalScore, &songHighScores.fadedGamesPlayed,
            &songHighScores.linkHighScore, &songHighScores.linkTotalScore, &songHighScores.linkGamesPlayed,
            &songHighScores.skyHighHighScore, &songHighScores.skyHighTotalScore, &songHighScores.skyHighGamesPlayed);
        fclose(file);
    }

    int played = 0;

    // Calculate the total score and games played based on the selected song
    switch (song) {
    case 0: // Faded
        played = songHighScores.fadedGamesPlayed;
        break;
    case 1: // Link
        played = songHighScores.linkGamesPlayed;
        break;
    case 2: // Sky High
        played = songHighScores.skyHighGamesPlayed;
        break;
    }

    return played;
}
