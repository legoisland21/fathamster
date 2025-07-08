#include <iostream>
#include "include/raylib.h"
#include "include/tinyfiledialogs.h"
#include <string>
#include <cmath>
using namespace std;
// Floats
float gravity = 0.5f;
const float jumpForce = -10.0f;
float velocityY = 0;
float gravityset;
float screenWidth = 1366;
float screenHeight = 768;

// Bools
bool infJump, unlockFps, showFps, moonJump, flop, isJump, devMode;

// Ints
int fps, maxfps, posX, posY;
int fpslimit = 60;

// Strings/Chars
string cheatcode;
const char* moon;
const char* code;
const char* fpsmenu;

// WinMain (I can't include windows.h)
typedef void* HINSTANCE;
typedef const char* LPCSTR;
typedef int BOOL;
typedef char* LPSTR;
typedef unsigned long DWORD;
#define WINAPI __stdcall

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	InitWindow(screenWidth, screenHeight, "Fat Hamster: THE GAME"); // Make window
	SetTargetFPS(fpslimit); // Set max FPS
	
	Vector2 position = { 0,400 }; // Player position
	Vector2 screen = { screenWidth, screenHeight }; // Screen size
	// Textures
	Texture2D floor = LoadTexture("textures/floor.jpg");
	Texture2D player = LoadTexture("textures/player.png");
	Texture2D player_ = LoadTexture("textures/player_flop.png");
	Texture2D floor_ = LoadTexture("textures/floor_flop.jpg");

	// Audio
	InitAudioDevice();
	Sound cheatOn = LoadSound("audio/cheatstart.wav");
	Sound cheatOff = LoadSound("audio/cheatstop.wav");
	Music music = LoadMusicStream("audio/music.mp3");
	Music music_ = LoadMusicStream("audio/music_.mp3");
	SetMusicVolume(music, 0.15f);
	SetMusicVolume(music_, 0.0f);
	PlayMusicStream(music);
	PlayMusicStream(music_);

	// Loop
	while (!WindowShouldClose()) {
		if(!flop) {
			UpdateMusicStream(music); // Loop music
		}
		else {
			UpdateMusicStream(music_);
		}
		
		if(!flop) {
			SetMusicVolume(music, 0.15f);
			SetMusicVolume(music_, 0.0f);
		}
		else {
			SetMusicVolume(music_, 0.15f);
			SetMusicVolume(music, 0.0f);
		}
		
		
		fps = GetFPS(); // FPS
		// Jump
		if (IsKeyPressed(KEY_SPACE)) {
			if (!infJump) {
				if (!isJump) {
					velocityY = jumpForce;
					isJump = true;
				}
			}
			else {
				velocityY = jumpForce;
				isJump = true;
			}
		}
		
		// Gravity
		velocityY += gravity;
		position.y += velocityY;
		
		// Controls
		if (IsKeyDown(KEY_A)) {
			if (position.x > 0) {
				position.x -= 7;
			}
		}
		if (IsKeyDown(KEY_D)) {
			if (position.x < screen.x-75) {
				position.x += 7;
			}
		}
		
		// Collision
		if (position.y >= 575) {
			position.y = 575;
			velocityY = 0;
			isJump = false;
		}
		
		
		// Dev mode
		if (IsKeyPressed(KEY_F7)) {
			devMode = !devMode;
			if (devMode) {
				PlaySound(cheatOn);
				cout << "Cheat mode activated" << endl;
			}
			else {
				PlaySound(cheatOff);
				cout << "Cheat mode disabled" << endl;
			}
		}
		if (IsKeyPressed(KEY_APOSTROPHE)) {
			if (devMode) {
				code = tinyfd_inputBox("Cheat Code", "Enter your cheat code:", "");
				string cheatcode(code);
				if (cheatcode == "infjump") {
					infJump = !infJump;
				}
				else if (cheatcode == "unlockfps") {
					unlockFps = !unlockFps;
					if (unlockFps) {
						SetTargetFPS(0.5);
					}
					else {
						SetTargetFPS(fpslimit);
					}
				}
				else if (cheatcode == "showfps") {
					showFps = !showFps;
				}
				else if (cheatcode == "changefps") {
					fpsmenu = tinyfd_inputBox("FPS code", "What fps:", "");
					fpslimit = stoi(fpsmenu);
					SetTargetFPS(fpslimit);
				}
				else if (cheatcode == "moon") {
					moonJump = !moonJump;
					moon = tinyfd_inputBox("Moon code", "What to set gravity (default 0.5f) (add f after float ex. 1.0f):", "");
					gravityset = stof(moon);
					gravity = gravityset;
				}
				else if (cheatcode == "flopper") {
					flop = !flop;
				}
			}
		}

		// Drawing
		BeginDrawing();
		if (!flop) {
			ClearBackground(SKYBLUE); 
		}
		else {
			ClearBackground({ 181, 171, 161, 1 }); // Flop cheat code
		}

		// Sun
		if (!flop) {
			DrawCircle(50, 50, 50, YELLOW);
		}
		else {
			DrawCircle(50, 50, 50, BEIGE); // Flop cheat code
		}

		if (showFps) {
			posX = static_cast<int>(position.x);
			posY = static_cast<int>(position.y);
			DrawText(TextFormat("Max FPS: %d", maxfps), 0, 0, 30, BLACK);
			DrawText(TextFormat("FPS: %d", fps), 0, 30, 30, BLACK);
			DrawText(TextFormat("X position: %d", posX), 0, 60, 30, BLACK);
			DrawText(TextFormat("Y position %d", posY - 575), 0, 90, 30, BLACK);
		}

		// Floor
		if (!flop) {
			DrawTexture(floor, 0, 650, WHITE);
		}
		else {
			DrawTexture(floor_, 0, 650, WHITE); // Flop cheat code
		}

		// Player
		if (!flop) {
			DrawTextureEx(player, position, 0.0f, 0.1f, WHITE);
		}
		else {
			DrawTextureEx(player_, position, 0.0f, 0.1f, WHITE); // Flop cheat code
		}
		EndDrawing();
		
		if (fps > maxfps) {
			maxfps = fps; // Max FPS
		}
	}

	// Unload sprites and audio
	UnloadSound(cheatOff);
	UnloadSound(cheatOn);
	UnloadMusicStream(music);
	UnloadMusicStream(music_);
	CloseAudioDevice();
	// Quit game
	CloseWindow();
	return -1;
}
