#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include "include/raylib.h"
#include "include/tinyfiledialogs.h"
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


string LowerCase(string& str) {
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return tolower(c); });
    return str;
}

void HandleCheats() {
    CodeUi = tinyfd_inputBox("Cheat code", "Enter your desired Cheat code:", "");
    if (CodeUi == NULL || CodeUi[0] == '\0') return; 
    string BasicCode(CodeUi);
    string Code = LowerCase(BasicCode);
    if(Code == "infjump") InfJump = !InfJump;
    else if (Code == "unlockfps") { UnlockFps = !UnlockFps; if (UnlockFps) SetTargetFPS(0.5); else SetTargetFPS(60); }
    else if (Code == "showfps") ShowFps = !ShowFps;
    else if (Code == "changefps") { FPSUi = tinyfd_inputBox("FPS cheat code", "How much FPS?:", "");  if(FPSUi == NULL || FPSUi[0] == '\0') return; LIMITFPS = stoi(FPSUi); SetTargetFPS(LIMITFPS);  }
    else if (Code == "moon") { MoonJump = !MoonJump; MoonUi = tinyfd_inputBox("Moon cheat code", "What gravity? (default 0.5f):", ""); if(MoonUi == NULL || MoonUi[0] == '\0') { Gravity = 0.5f; return; } else Gravity = stof(MoonUi); }
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Fat Hamster: The Game");
    Texture2D Sky = LoadTexture("textures/background.jpg");
    Texture2D Ground = LoadTexture("textures/ground.jpg");
    Texture2D Player = LoadTexture("textures/player.png");
    Texture2D PlayerJump = LoadTexture("textures/player_jump.png");
    if(Sky.id == 0 || Ground.id == 0 || Player.id == 0 || PlayerJump.id == 0) { tinyfd_messageBox("ERROR", "Error: Textures not loaded, game will quit now." , "ok", "error", 1); return -1; }
    InitAudioDevice();
    Sound CheatOn = LoadSound("audio/cheat_on.mp3");
    Sound CheatOff = LoadSound("audio/cheat_off.mp3");
    Music BGM = LoadMusicStream("audio/bgm.mp3");
    if(CheatOn.frameCount == 0 || CheatOff.frameCount == 0 ) { tinyfd_messageBox("ERROR", "Error: Sounds not loaded, game will quit now." , "ok", "error", 1); return -1; }
    if(BGM.ctxData == NULL) { tinyfd_messageBox("ERROR", "Error: Music not loaded, game will quit now." , "ok", "error", 1); return -1; }
    PlayMusicStream(BGM);
    SetMusicVolume(BGM, 0.15f);

    Vector2 PlayerPos = {0, ScreenHeight - GroundLevel};

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        UpdateMusicStream(BGM);
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

        if(IsKeyPressed(KEY_F9)) { DevMode = !DevMode; if (DevMode) PlaySound(CheatOn); else PlaySound(CheatOff);}

        if(IsKeyPressed(KEY_APOSTROPHE) && DevMode) {
            HandleCheats();
        }

        BeginDrawing();

        Rectangle SourceRect = { 0.0f, 0.0f, (float)(FacingRight ? 1200 : -1200), 1200.0f };
        Rectangle DestRect = { PlayerPos.x, PlayerPos.y, 100, 100 };

        ClearBackground(RAYWHITE);
        DrawTextureEx(Sky, { 0, 0 } , 0.0f, SkyScale, WHITE);

        for (int i = 0; i != 7; i++) { DrawTextureEx(Ground, {0 + (i * 250.0f), ScreenHeight - GroundLevel + 100}, 0.0f, GroundScale, WHITE); }

        if (!Jumping && !InfJumping) {
            DrawTexturePro(Player, SourceRect, DestRect, {0,0}, 0.0f, WHITE);
        }
        else if (Jumping || InfJumping) {
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
            DrawText(TextFormat("Gravity: %.4g", Gravity), 0, 90, 30, BLACK);
            DrawText(TextFormat("Velocity (Y): %.4g", Velocity), 250, 90, 30, BLACK);
            if (IsKeyDown(KEY_SPACE)) DrawRectangle(1212, 110, 100, 20, WHITE);
            if (IsKeyDown(KEY_W)) DrawRectangle(1250, 50, 25, 25, WHITE);
            if (IsKeyDown(KEY_S)) DrawRectangle(1250, 80, 25, 25, WHITE);
            if (IsKeyDown(KEY_A)) DrawRectangle(1220, 80, 25, 25, WHITE);
            if (IsKeyDown(KEY_D)) DrawRectangle(1280, 80, 25, 25, WHITE);
        }
        EndDrawing();
    }
    UnloadMusicStream(BGM);
    UnloadSound(CheatOn);
    UnloadSound(CheatOff);
    UnloadTexture(Player);
    UnloadTexture(PlayerJump);
    UnloadTexture(Sky);
    UnloadTexture(Ground);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}