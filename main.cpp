#include <iostream>
#include <array>
#include <list> 
#include <SDL.h>
#include <SDL_image.h>

#include "objects.h"

using namespace std;

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 736;

SDL_Window* window = NULL;
SDL_Renderer* game_renderer = NULL;
SDL_Texture* sprite_sheet = NULL;

int init(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL has returned an error initializing" << endl;
		return 0;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "SDL was unable to initialize imaging" << endl;
		return 0;
	}
	window = SDL_CreateWindow( "Jump Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		cout << "SDL was unable to create the window" << endl;
		return 0;
	}
	game_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!game_renderer){
		cout << "SDL was unable to create the game renderer" << endl;
		return 0;
	}
	SDL_SetRenderDrawColor(game_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//loads the sprite sheet as a texture
	SDL_Surface* image = NULL;
	image = IMG_Load("images/spritesheet.png");
	if(image == NULL){
		cout << "could not load image" << endl;
		return 0;
	}
	sprite_sheet = SDL_CreateTextureFromSurface(game_renderer, image);
	if(sprite_sheet == NULL){
		cout << "could not convert image to texture" << endl;
		return 0;
	}
	SDL_FreeSurface(image);

	return 1;
}

void clean_up(){
	SDL_DestroyRenderer(game_renderer);
	game_renderer = NULL;
	SDL_DestroyTexture(sprite_sheet);
	sprite_sheet = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
    SDL_Quit();
}

SDL_Rect create_sky(){
	SDL_Rect sky;
	sky.x = 0;
	sky.y = 32;
	sky.w = 32;
	sky.h = 32;
	return sky;
}

int main(int argv, char* args[]){
	if(!init())
		return 1;

	Player* player = new Player(20, 300, 32, 32);

	list<Brick*> bricks;
	for(int i = 0; i < SCREEN_WIDTH/16; i++)
		bricks.push_back(new Brick(i*16, 332, 16, 16));

	SDL_Rect sky_rect = create_sky();

	bool exit = false;
	SDL_Event e;
	int frame = 0;
	while(!exit){
		//finds out which keys have been pressed
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				exit = true;
		}
		//sees if the player is being moved at all
		const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
		array<bool, 4> states = {false, false, false, false};
		if(current_key_states[SDL_SCANCODE_UP])
			states[UP] = true;
		if(current_key_states[SDL_SCANCODE_DOWN])
			states[DOWN] = true;
		if(current_key_states[SDL_SCANCODE_LEFT])
			states[LEFT] = true;
		if(current_key_states[SDL_SCANCODE_RIGHT])
			states[RIGHT] = true;
		player->update(frame, states);

		SDL_RenderClear(game_renderer);

		//renders the sky
		SDL_Rect sky_dest;
		for(int i = 0; i < SCREEN_HEIGHT/32; i++)
			for(int j = 0; j < SCREEN_WIDTH/32; j++){
				sky_dest = (SDL_Rect){j*32, i*32, 32, 32};
				SDL_RenderCopy(game_renderer, sprite_sheet, &(sky_rect), &(sky_dest));
			}

		//renders the bricks
		for(list<Brick*>::iterator i = bricks.begin(); i != bricks.end(); ++i)
			SDL_RenderCopy(game_renderer, sprite_sheet, &((*i)->src_rect), &((*i)->dest_rect));

		//render player
		SDL_RenderCopy(game_renderer, sprite_sheet, &(player->src_rect), &(player->dest_rect));
		
		//update screen
		SDL_RenderPresent(game_renderer);

		frame = (frame / 4 == 4) ? 0 : frame + 1;
	}

	delete player;
	bricks.clear();
	clean_up();
	return 0;
}

