#pragma once

#include "SDL/include/SDL.h"
#include "SDL_Image/include/SDL_image.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma once

#include "SDL/include/SDL.h"
#include "SDL_Image/include/SDL_image.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include "Entity.h"

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768 
#define MAX_KEYS		256
#define MAX_SHOTS		32

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Release();
	bool LoadImages();

	bool Input();
	bool Update();
	void Draw();

	bool valid[4] = { true, false, false, false };


private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	SDL_Texture* img_player, * img_shot, * img_background, * img_luffy2, * img_luffy3, * img_luffy4, * img_puny2, * img_puny3, * img_puny4;

	SDL_Surface* surface;
	//SDL_Texture* Luffy2;



	Entity Player, Shots1[MAX_SHOTS], Shots2[MAX_SHOTS], Scene;
	int idx_shot;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};