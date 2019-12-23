#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "player.h"

using namespace std;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 720;

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
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	image = IMG_Load("images/player.png");
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

int main(int argv, char* args[]){
	if(!init())
		return 1;

	Player* player = new Player(20, 20, 32, 32);

	bool exit = false;
	SDL_Event e;
	int frame = 0;
	while(!exit){
		//finds out which keys have been pressed
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				exit = true;
		}
		SDL_RenderClear(game_renderer);
		//render player
		player->update_animation(frame);
		SDL_RenderCopy(game_renderer, sprite_sheet, &(player->src_rect), &(player->dest_rect));
		//update screen
		SDL_RenderPresent(game_renderer);

		frame = (frame / 4 == 4) ? 0 : frame + 1;
	}

	delete player;
	clean_up();
	return 0;
}

