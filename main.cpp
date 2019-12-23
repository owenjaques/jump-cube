#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "player.h"

using namespace std;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 720;

int main(int argv, char* args[]){
	SDL_Window* window = NULL;
	SDL_Renderer* game_renderer = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL has returned an error initializing" << endl;
		return 1;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "SDL was unable to initialize imaging" << endl;
		return 1;
	}
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		cout << "SDL was unable to create the window" << endl;
		return 1;
	}
	game_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!game_renderer){
		cout << "SDL was unable to create the game renderer" << endl;
	}
	SDL_SetRenderDrawColor(game_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	Player* player = new Player(20, 20, 32, 32, "images/player.png", game_renderer);

	bool exit = false;
	SDL_Event e;
	while(!exit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				exit = true;
		}
		SDL_RenderClear(game_renderer);
		//Render texture to screen
		SDL_RenderCopy(game_renderer, player->image, NULL, &(player->dest_rect));
		//Update screen
		SDL_RenderPresent(game_renderer);
	}
	
	//clean up
	delete player;
	SDL_DestroyRenderer(game_renderer);
	game_renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
    SDL_Quit();
	return 0;
}

