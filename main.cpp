#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "include/raylib.h"
#include "include/tinyfiledialogs.h"
using namespace std;

bool InfJump, UnlockFps, ShowFps, MoonJump, DevMode;
int TempGravity;

const int ScreenWidth = 1366;
const int ScreenHeight = 768;
const int GroundLevel = 200;
const float PlayerScale = 12.0f;
const float GroundScale = 0.15f;
const float SkyScale = 1.0f;
float Gravity = 0.5f;
const float JumpForce = -10.0f;
float Velocity = 0;
bool Jumping, InfJumping;
bool FacingRight = true;

int x,y;

const char* CodeUi;
const char* MoonUi;
const char* FPSUi;

int FPS, MFPS, LIMITFPS;

void LogToFile(string log) {
    fstream logFile("gamelog.log", ios::out | ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back();

    logFile << "(" << timestamp << "): " << log << endl;
    logFile.close();
}

void HandleCheats() {
    LogToFile("Opened HandleCheats function");
    CodeUi = tinyfd_inputBox("Cheat code", "Enter your desired Cheat code:", "");
    if (CodeUi == NULL) { LogToFile("Error: CodeUi is NULL"); return; }
    else if (CodeUi[0] == '\0') { LogToFile("Error: CodeUi is Empty"); return; }
    string Code(CodeUi);
    if(Code == "InfJump") { InfJump = !InfJump; LogToFile("InfJump is " + string((InfJump ? "true" : "false")));}
    else if (Code == "UnlockFPS") { UnlockFps = !UnlockFps; if (UnlockFps) SetTargetFPS(0.5); else SetTargetFPS(60); LogToFile("UnlockFPS is " + string((UnlockFps ? "true" : "false"))); }
    else if (Code == "ShowFPS") { ShowFps = !ShowFps; LogToFile("ShowFPS is " + string((ShowFps ? "true" : "false"))); }
    else if (Code == "ChangeFPS") { FPSUi = tinyfd_inputBox("FPS cheat code", "How much FPS?:", "");  if(FPSUi == NULL) { LogToFile("Error: FPSUi is NULL"); return; } if(FPSUi[0] == '\0') { LogToFile("Error: FPSUi is Empty"); return; } LIMITFPS = stoi(FPSUi); SetTargetFPS(LIMITFPS); LogToFile("FPS is now " + to_string(LIMITFPS)); }
    else if (Code == "Moon") { MoonJump = !MoonJump; MoonUi = tinyfd_inputBox("Moon cheat code", "What gravity? (default 0.5f):", ""); if(MoonUi == NULL) { LogToFile("Error: MoonUi is NULL"); return; } if(MoonUi[0] == '\0') { LogToFile("Error: MoonUi is Empty"); return; } TempGravity = stof(MoonUi); Gravity = TempGravity; LogToFile("Gravity is now " + to_string(TempGravity)); }
}

int main() {
    LogToFile("===================================");
    LogToFile("Opened Game");
    InitWindow(ScreenWidth, ScreenHeight, "Fat Hamster: The Game"); LogToFile("Initiated Window");
    SetTargetFPS(60);
    Texture2D Sky = LoadTexture("textures/background.jpg");
    Texture2D Ground = LoadTexture("textures/ground.jpg");
    Texture2D Player = LoadTexture("textures/player.png");
    Texture2D PlayerJump = LoadTexture("textures/player_jump.png");
    LogToFile("Loaded Textures");

    InitAudioDevice();
    Sound CheatOff = LoadSound("audio/cheat_off.mp3");
    Sound CheatOn = LoadSound("audio/cheat_on.mp3");
    Music BackgroundMusic = LoadMusicStream("audio/music.mp3");
    PlayMusicStream(BackgroundMusic);
    SetMusicVolume(BackgroundMusic, 0.15f);
    LogToFile("Loaded Audio");

    Vector2 PlayerPos = {0, ScreenHeight - GroundLevel};
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

        if(IsKeyDown(KEY_D) && PlayerPos.x < 1366 - Player.height / PlayerScale) { PlayerPos.x += 5; FacingRight = true; }
        if(IsKeyDown(KEY_A) && PlayerPos.x > 0) { PlayerPos.x -= 5; FacingRight = false; }


        if(IsKeyPressed(KEY_F7)) { if(!DevMode) PlaySound(CheatOn); else PlaySound(CheatOff); DevMode = !DevMode; LogToFile("DevMode is " + string((DevMode ? "true" : "false"))); }
        
        if(IsKeyPressed(KEY_APOSTROPHE) && DevMode) {
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

        x = PlayerPos.x;
        y = PlayerPos.y;
        if(y >= 0) y *= 2;

        if(ShowFps) {
            DrawText(TextFormat("FPS: %d", FPS), 0, 0, 30, BLACK);
            DrawText(TextFormat("Max FPS: %d", MFPS), 0, 30, 30, BLACK);
            DrawText(TextFormat("X Position: %d", x), 0, 60, 30, BLACK);
            DrawText(TextFormat("Y Position: %d", y), 0, 90, 30, BLACK);
        }
        EndDrawing();
    }
    UnloadSound(CheatOff);
    UnloadSound(CheatOn);
    UnloadMusicStream(BackgroundMusic);
    UnloadTexture(Player);
    UnloadTexture(PlayerJump);
    UnloadTexture(Sky);
    UnloadTexture(Ground);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}