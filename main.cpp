#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include "include/raylib.h"
#include "include/tinyfiledialogs.h"
#include "include/logger_custom.h"
using namespace std;

// Strings / Chars
const char* CodeUi;
const char* MoonUi;
const char* FPSUi;

// Bools
bool InfJump, UnlockFps, ShowFps, MoonJump, DevMode;
bool Jumping, InfJumping;
bool FacingRight = true;

// Ints / Floats
int TempGravity;
int x, y, MouseX, MouseY;
int FPS, MFPS, LIMITFPS;
const int ScreenWidth = 1366;
const int ScreenHeight = 768;
const int GroundLevel = 200;
const int WalkSpeed = 7;
const float PlayerScale = 12.0f;
const float GroundScale = 0.15f;
const float SkyScale = 1.0f;
const float JumpForce = -10.0f;
float Gravity = 0.5f;
float Velocity = 0;
Logger logger("game.log");


string LowerCase(string& str) {
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return tolower(c); });
    return str;
}

void HandleCheats() {
    CodeUi = tinyfd_inputBox("Cheat code", "Enter your desired Cheat code:", "");
    if (CodeUi == NULL || CodeUi[0] == '\0') { logger.LogToFile("CodeUi is NULL/empty", 3); return; }
    string BasicCode(CodeUi);
    string Code = LowerCase(BasicCode);
    if(Code == "infjump") { InfJump = !InfJump; logger.LogToFile("Infjump is " + to_string(InfJump) + ".", 1);}
    else if (Code == "unlockfps") { UnlockFps = !UnlockFps; if (UnlockFps) SetTargetFPS(0.5); else SetTargetFPS(60); logger.LogToFile("Unlockfps is " + to_string(UnlockFps) + ".", 1); }
    else if (Code == "showfps") { ShowFps = !ShowFps; logger.LogToFile("Showfps is " + to_string(ShowFps) + ".", 1); }
    else if (Code == "changefps") { FPSUi = tinyfd_inputBox("FPS cheat code", "How much FPS?:", "");  if(FPSUi == NULL || FPSUi[0] == '\0') { logger.LogToFile("FPSUi is NULL/Empty.", 2); return; } LIMITFPS = stoi(FPSUi); SetTargetFPS(LIMITFPS); logger.LogToFile("Max FPS is " + to_string(LIMITFPS) + ".", 1); }
    else if (Code == "moon") { MoonJump = !MoonJump; MoonUi = tinyfd_inputBox("Moon cheat code", "What gravity? (default 0.5f):", ""); if(MoonUi == NULL || MoonUi[0] == '\0') { Gravity = 0.5f; logger.LogToFile("MoonUi is NULL/Empty.", 2); return; } else { Gravity = stof(MoonUi); } logger.LogToFile("Gravity is " + to_string(Gravity) + ".", 1); }
}

int main() {
    logger.LogToFile("========== Fat Hamster: The Game ==========", 0);
    InitWindow(ScreenWidth, ScreenHeight, "Fat Hamster: The Game");
    logger.LogToFile("Initiated Window.", 1);
    Texture2D Sky = LoadTexture("textures/background.jpg");
    Texture2D Ground = LoadTexture("textures/ground.jpg");
    Texture2D Player = LoadTexture("textures/player.png");
    Texture2D PlayerJump = LoadTexture("textures/player_jump.png");

    InitAudioDevice();
    Sound CheatOn = LoadSound("audio/cheat_on.mp3");
    Sound CheatOff = LoadSound("audio/cheat_off.mp3");
    Music BackgroundMusic = LoadMusicStream("audio/music.mp3");
    PlayMusicStream(BackgroundMusic);
    SetMusicVolume(BackgroundMusic, 0.15f);
    logger.LogToFile("Loaded Resources.", 1);

    Vector2 PlayerPos = {0, ScreenHeight - GroundLevel};

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        UpdateMusicStream(BackgroundMusic);
        FPS = GetFPS();

        if (IsKeyPressed(KEY_SPACE)) {
			if(!Jumping) {
                Velocity = JumpForce;
                if (!InfJump) Jumping = true;
                else { InfJumping = true; if(Jumping) Jumping = false;}
            }
		}

        Velocity += Gravity;
		PlayerPos.y += Velocity;
        
        if (PlayerPos.y >= ScreenHeight - GroundLevel) {
			PlayerPos.y = ScreenHeight - GroundLevel;
			Velocity = 0;
			Jumping = false;
			InfJumping = false;
		}

        if(IsKeyDown(KEY_D) && PlayerPos.x < 1366 - Player.height / PlayerScale) { PlayerPos.x += WalkSpeed; FacingRight = true; }
        if(IsKeyDown(KEY_A) && PlayerPos.x > 0) { PlayerPos.x -= WalkSpeed; FacingRight = false; }

        if(IsKeyPressed(KEY_F9)) { DevMode = !DevMode; if (DevMode) PlaySound(CheatOn); else PlaySound(CheatOff); logger.LogToFile("Devmode is " + to_string(DevMode) + ".", 0);}

        if(IsKeyPressed(KEY_APOSTROPHE) && DevMode) {
            logger.LogToFile("Opening Cheats...", 1);
            HandleCheats();
        }

        BeginDrawing();

        Rectangle SourceRect = { 
            0.0f, 0.0f,
            (float)(FacingRight ? 1200 : -1200),
            1200.0f
        };
        Rectangle DestRect = { PlayerPos.x, PlayerPos.y, 100, 100 };

        ClearBackground(RAYWHITE);
        DrawTextureEx(Sky, { 0, 0 } , 0.0f, SkyScale, WHITE);

        for (int i = 0; i != 7; i++) { DrawTextureEx(Ground, {0 + (i * 250.0f), ScreenHeight - GroundLevel + 100}, 0.0f, GroundScale, WHITE); }

        if (!Jumping && !InfJumping) {
            DrawTexturePro(Player, SourceRect, DestRect, {0,0}, 0.0f, WHITE);
        }
        if (InfJumping) {
            DrawTexturePro(PlayerJump, SourceRect, DestRect, {0,0}, 0.0f, WHITE);
        }
        if (Jumping) {
            DrawTexturePro(PlayerJump, SourceRect, DestRect, {0,0}, 0.0f, WHITE);
        }
        if (FPS > MFPS) MFPS = FPS;
        if(ShowFps) {
            x = PlayerPos.x;
            MouseX = GetMouseX();
            y = PlayerPos.y;
            MouseY = GetMouseY();
            if(y >= 0) y *= 2;
            DrawText(TextFormat("FPS: %d", FPS), 0, 0, 30, BLACK);
            DrawText(TextFormat("Max FPS: %d", MFPS), 250, 0, 30, BLACK);
            DrawText(TextFormat("X Position: %d", x), 0, 30, 30, BLACK);
            DrawText(TextFormat("Y Position: %d", y), 250, 30, 30, BLACK);
            DrawText(TextFormat("Mouse X: %d", MouseX), 0, 60, 30, BLACK);
            DrawText(TextFormat("Mouse Y: %d", MouseY), 250, 60, 30, BLACK);
            if (IsKeyDown(KEY_SPACE)) DrawRectangle(1212, 110, 100, 20, WHITE);
            if (IsKeyDown(KEY_W)) DrawRectangle(1250, 50, 25, 25, WHITE);
            if (IsKeyDown(KEY_S)) DrawRectangle(1250, 80, 25, 25, WHITE);
            if (IsKeyDown(KEY_A)) DrawRectangle(1220, 80, 25, 25, WHITE);
            if (IsKeyDown(KEY_D)) DrawRectangle(1280, 80, 25, 25, WHITE);
        }
        EndDrawing();
    }
    UnloadMusicStream(BackgroundMusic);
    UnloadSound(CheatOn);
    UnloadSound(CheatOff);
    UnloadTexture(Player);
    UnloadTexture(PlayerJump);
    UnloadTexture(Sky);
    UnloadTexture(Ground);
    logger.LogToFile("Unloaded Resources", 1);
    CloseAudioDevice();
    CloseWindow();
    logger.LogToFile("Game Closed.", 0);
    return 0;
}