#include <iostream>
#include "raylib.h"
#include "tinyfiledialogs.h"
#include <string>
using namespace std;
bool doDraw = true;
bool isJump = false;
bool devMode = false;
float gravity = 0.5f;
const float jumpForce = -10.0f;
float velocityY = 0;
bool infJump, unlockFps, showFps, moonJump;
int fps, maxfps;
int fpslimit = 60;
float gravityset;

typedef void* HINSTANCE;
typedef const char* LPCSTR;
typedef int BOOL;
typedef char* LPSTR;
typedef unsigned long DWORD;

#define WINAPI __stdcall

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	InitWindow(1280, 720, "Fat Hamster: THE GAME");
	SetTargetFPS(fpslimit);
	// VARIABLES
	Vector2 position = { 0,500 };
	Vector2 screen = { 1280, 720 };
	Texture2D floor = LoadTexture("textures/stuff.jpg");
	Texture2D player = LoadTexture("textures/robin.png");
	InitAudioDevice();
	Sound cheatOn = LoadSound("audio/cheatstart.wav");
	Sound cheatOff = LoadSound("audio/cheatstop.wav");
	Music music = LoadMusicStream("audio/music.wav");
	SetMusicVolume(music, 0.15f);
	PlayMusicStream(music);
	float wheel = GetMouseWheelMove();
	string cheatcode;
	const char* moon;
	const char* code;
	const char* fpsmenu;
	int posX;
	int posY;
	// CAMERA
	Camera2D camera = { 0 };
	camera.target = { position.x + 20, position.y + 20 };
	camera.offset = { 1280 / 2.0f, 720 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	// CAMERA
	// MAIN LOOP
	while (!WindowShouldClose()) {
		UpdateMusicStream(music);
		fps = GetFPS();
		// JUMP THING (HAD TO BE SEPERATERD)
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
		// JUMP THING (HAD TO BE SEPERATED)
		// GRAVITY
		velocityY += gravity;
		position.y += velocityY;
		// GRAVITY
		// CONTROLS
		if (IsKeyDown(KEY_A)) {
			if (position.x > -12000) {
				position.x -= 7;
			}
		}
		if (IsKeyDown(KEY_D)) {
			if (position.x < 12000) {
				position.x += 7;
			}
		}

		wheel = GetMouseWheelMove();
		if (wheel != 0) {
			camera.zoom += wheel * 0.1f;
			if (camera.zoom > 5.0) camera.zoom = 5.0f;
			if (camera.zoom < 0.1) camera.zoom = 0.1f;
		}
		camera.target = { position.x + 20, position.y + 20 };
		// CONTROLS
		// GROUND COLLISION
		if (position.y >= 575) {
			position.y = 575;
			velocityY = 0;
			isJump = false;
		}
		// GROUND COLLISION
		// CAMERA
		// CAMERA
		// DEV MODE
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
		// DEV MODE
		// CHEAT CODE STUFF
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
			}
		}
		// CHEAT CODE STUFF
		// START DRAWING
		BeginDrawing();
		ClearBackground(SKYBLUE);
		if (showFps) {
			posX = static_cast<int>(position.x);
			posY = static_cast<int>(position.y);
			DrawText(TextFormat("maxfps %d", maxfps), 0, 0, 30, BLACK);
			DrawText(TextFormat("fps %d", fps), 0, 30, 30, BLACK);
			DrawText(TextFormat("x %d", posX), 0, 60, 30, BLACK);
			DrawText(TextFormat("y %d", posY-575), 0, 90, 30, BLACK);
		}
		BeginMode2D(camera);
		DrawCircle(50, 50, 50, YELLOW);
		float groundOffsetX = fmod(camera.target.x, floor.width);
		for (int i = -10; i < screen.x / floor.width + 70; i++) {
			DrawTexture(floor, (int)(i * floor.width - groundOffsetX), 650, WHITE);
		}
		//DrawTexture(floor, 0, 650, WHITE);
		DrawTextureEx(player, position, 0.0f, 0.1f, WHITE);
		EndMode2D();
		EndDrawing();
		// STOP DRAWING
		if (fps > maxfps) {
			maxfps = fps;
		}
	}
	// MAIN LOOP
	UnloadSound(cheatOff);
	UnloadSound(cheatOn);
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
}