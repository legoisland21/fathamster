#include <iostream>
#include <string>
#include "include/raylib.h"
#include "include/tinyfiledialogs.h"
using namespace std;

bool InfJump, UnlockFps, ShowFps, MoonJump, DevMode;
int TempGravity;

const int screenWidth = 1366;
const int screenHeight = 768;
const int groundLevel = 200;
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

void HandleCheats() {
    CodeUi = tinyfd_inputBox("Cheat code", "Enter your desired Cheat code:", "");
    if (CodeUi == NULL) return;
    else if (CodeUi[0] == '\0') return;
    string Code(CodeUi);
    if(Code == "InfJump") InfJump = !InfJump;
    else if (Code == "UnlockFPS") { UnlockFps = !UnlockFps; if (UnlockFps) SetTargetFPS(0.5); else SetTargetFPS(60); }
    else if (Code == "ShowFPS") { ShowFps = !ShowFps; }
    else if (Code == "ChangeFPS") { FPSUi = tinyfd_inputBox("FPS cheat code", "How much FPS?:", "");  if(FPSUi == NULL) return; if(FPSUi[0] == '\0') return; LIMITFPS = stoi(FPSUi); SetTargetFPS(LIMITFPS); }
    else if (Code == "Moon") { MoonJump = !MoonJump; MoonUi = tinyfd_inputBox("Moon cheat code", "What gravity? (default 0.5f):", ""); if(MoonUi == NULL) return; if(MoonUi[0] == '\0') return; TempGravity = stof(MoonUi); Gravity = TempGravity; }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Fat Hamster: The Game");
    SetTargetFPS(60);
    Texture2D Sky = LoadTexture("textures/background.jpg");
    Texture2D Ground = LoadTexture("textures/ground.jpg");
    Texture2D Player = LoadTexture("textures/player.png");
    Texture2D PlayerJump = LoadTexture("textures/player_jump.png");

    InitAudioDevice();
    Sound CheatOff = LoadSound("audio/cheat_off.mp3");
    Sound CheatOn = LoadSound("audio/cheat_on.mp3");
    Music BackgroundMusic = LoadMusicStream("audio/music.mp3");
    PlayMusicStream(BackgroundMusic);
    SetMusicVolume(BackgroundMusic, 0.15f);

    Vector2 PlayerPos = {0, screenHeight - groundLevel};

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
        
        if (PlayerPos.y >= screenHeight - groundLevel) {
			PlayerPos.y = screenHeight - groundLevel;
			Velocity = 0;
			Jumping = false;
			InfJumping = false;
		}

        if(IsKeyDown(KEY_D) && PlayerPos.x < 1366 - 100) { PlayerPos.x += 5; FacingRight = true; }
        if(IsKeyDown(KEY_A) && PlayerPos.x > 0) { PlayerPos.x -= 5; FacingRight = false; }


        if(IsKeyPressed(KEY_F7)) { if(!DevMode) PlaySound(CheatOn); else PlaySound(CheatOff); DevMode = !DevMode;}
        
        if(IsKeyPressed(KEY_APOSTROPHE) && DevMode) {
            HandleCheats();
        }

        BeginDrawing();

        Rectangle sourceRect = { 
            0.0f, 0.0f,
            (float)(FacingRight ? 1200 : -1200),
            1200.0f
        };
        Rectangle destRect = { PlayerPos.x, PlayerPos.y, 100, 100 };

        ClearBackground(RAYWHITE);
        DrawTexture(Sky, 0, 0, WHITE);

        for (int i = 0; i != 7; i++) { DrawTextureEx(Ground, {0 + (i * 250.0f), screenHeight - groundLevel + 100}, 0.0f, 0.15f, WHITE); }

        if (!Jumping && !InfJumping) {
            DrawTexturePro(Player, sourceRect, destRect, {0,0}, 0.0f, WHITE);
        }

        if (InfJumping) {
            DrawTexturePro(PlayerJump, sourceRect, destRect, {0,0}, 0.0f, WHITE);
        }

        if (Jumping) {
            DrawTexturePro(PlayerJump, sourceRect, destRect, {0,0}, 0.0f, WHITE);
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