#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game() {}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	//Init Image
	//Init variables
	
	//int x, y, w, h;
	//Player.GetRect(&x, &y, &w, &h);
	Player.Init(75, 38, 139, 190, 5);

	//Player.Init(20, WINDOW_HEIGHT >> 1, 104, 82, 5);
	idx_shot = 0;

	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, 1024, WINDOW_HEIGHT, 4);
	return true;
}

bool Game::LoadImages()
{
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}

	//escenaris
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("escenari_1.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	//luffys
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("luffy normal.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	surface = IMG_Load("luffy2normal.png");
	img_luffy2 = SDL_CreateTextureFromSurface(Renderer, surface);

	/*
	img_luffy2 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("luffy2normal.png"));
	if (img_luffy2 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}*/

	surface = IMG_Load("luffy3normal.png");
	img_luffy3 = SDL_CreateTextureFromSurface(Renderer, surface);

	surface = IMG_Load("luffy4normal.png");
	img_luffy4 = SDL_CreateTextureFromSurface(Renderer, surface);
	/*
	img_luffy3 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("luffy3normal.png"));
	if (img_luffy3 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_luffy4 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("luffy4normal.png"));
	if (img_luffy4 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	*/

	//punys
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("puny1.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_puny2 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("atac2.png"));
	if (img_puny2 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_puny3 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("puny3.png"));
	if (img_puny3 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_puny4 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("puny4.png"));
	if (img_puny4 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}





	return true;
}

void Game::Release()
{

	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_luffy2);
	SDL_DestroyTexture(img_luffy3);
	SDL_DestroyTexture(img_luffy4);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_puny2);
	SDL_DestroyTexture(img_puny3);
	SDL_DestroyTexture(img_puny4);

	IMG_Quit();
	//Clean up all SDL initialized subsystems
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}
bool Game::Update()
{
	SDL_Rect rc;

	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		Shots1[idx_shot].Init(x + w - 75, y + (h >> 1) - 40, 70, 60, 10);
		Shots2[idx_shot].Init(x + w - 75, y + (h >> 1) + 17, 70, 60, 10);

		idx_shot++; 
		idx_shot %= MAX_SHOTS;
	}

	//player
	
//	if (keys[SDL_SCANCODE_2] == KEY_DOWN)
	//{
		//Shots2[idx_shot].Init(x + w - 75, y + (h >> 1) + 17, 56, 20, 10);

		//idx_shot++;
		//idx_shot %= MAX_SHOTS;
//	}



	//Logic
	//Scene.Move(-1, 0);
	//if (Scene.GetX() <= -Scene.GetWidth())	Scene.SetX(0);
	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots1[i].IsAlive())
		{
			Shots1[i].Move(1, 0);
			if (Shots1[i].GetX() > WINDOW_WIDTH)	Shots1[i].ShutDown();

		}
		if (Shots2[i].IsAlive())
		{
			Shots2[i].Move(1, 0);
			if (Shots2[i].GetX() > WINDOW_WIDTH)	Shots2[i].ShutDown();
		}
	}

	//Player
	/*if (Player.IsAlive())
	{
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		//if (Shots1[i].GetX() > WINDOW_WIDTH)	Shots1[i].ShutDown();

	}*/

	return false;
}
void Game::Draw()
{
	SDL_Rect rc;
	//SDL_Rect Luffy = { 20,20,50,30 };

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//Draw scene
	Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);
	rc.x += rc.w;
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);

	//Draw player normal
	if (valid[0]==true&&valid[1]==false&&valid[2]==false&&valid[3]==false) {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);

		SDL_RenderCopy(Renderer, img_player, NULL, &rc);

		SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots1[i].IsAlive())
			{
				Shots1[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
			}
			if (Shots2[i].IsAlive())
			{
				Shots2[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
			}
		}
	}

	if (valid[1] == true) {
		//Player.Init(rc.x, rc.y, rc.w, rc.h, 5);
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);


		SDL_RenderCopy(Renderer, img_luffy2, NULL, &rc);

		SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots1[i].IsAlive())
			{
				Shots1[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny2, NULL, &rc);
			}
			if (Shots2[i].IsAlive())
			{
				Shots2[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny2, NULL, &rc);
			}
		}

	}
	if (valid[2] == true) {
		//Player.Init(rc.x, rc.y, rc.w, rc.h, 5);
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);


		SDL_RenderCopy(Renderer, img_luffy3, NULL, &rc);

		SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots1[i].IsAlive())
			{
				Shots1[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny3, NULL, &rc);
			}
			if (Shots2[i].IsAlive())
			{
				Shots2[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny3, NULL, &rc);
			}
		}

	}
	if (valid[3] == true) {
		//Player.Init(rc.x, rc.y, rc.w, rc.h, 5);
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);


		SDL_RenderCopy(Renderer, img_luffy4, NULL, &rc);
		SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots1[i].IsAlive())
			{
				Shots1[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny4, NULL, &rc);
			}
			if (Shots2[i].IsAlive())
			{
				Shots2[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_puny4, NULL, &rc);
			}
		}

	}

	//SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

	//Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);

	
	//Draw players
	
	if (keys[SDL_SCANCODE_1] == KEY_DOWN) {
		valid[0] = true;
		valid[1] = false;
		valid[2] = false;
		valid[3] = false;
	}else if (keys[SDL_SCANCODE_2] == KEY_DOWN) {
		valid[0] = false;
		valid[1] = true;
		valid[2] = false;
		valid[3] = false;
	}else if (keys[SDL_SCANCODE_3] == KEY_DOWN) {
		valid[0] = false;
		valid[1] = false;
		valid[2] = true;
		valid[3] = false;
	}else if (keys[SDL_SCANCODE_4] == KEY_DOWN) {
		valid[0] = false;
		valid[1] = false;
		valid[2] = false;
		valid[3] = true;
	}



	//SDL_RenderFillRect(Renderer, &rc);



	//Draw shots
	/*
	SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots1[i].IsAlive())
		{
			Shots1[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
		}
		if (Shots2[i].IsAlive())
		{
			Shots2[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
		}
	}
	*/
	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}