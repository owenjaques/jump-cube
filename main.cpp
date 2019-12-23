#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "player.h"

using namespace std;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 720;

int main(int argv, char* args[]){
	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL has returned an error initializing\n";
		return 1;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "SDL was unable to initialize imaging\n";
		return 1;
	}
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		cout << "SDL was unable to create the window\n";
		return 1;
	}

	screen_surface = SDL_GetWindowSurface(window);
	Player* player = new Player(20, 20, 32, 32, "images/player.png", screen_surface);

	bool exit = false;
	SDL_Event e;
	while(!exit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				exit = true;
		}
		SDL_BlitScaled(player->image, NULL, screen_surface, &(player->stretch_rect));
		SDL_UpdateWindowSurface(window);
	}
	
	//clean up
	delete player;
	SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

