#include <stdio.h>
#include <SDL.h>

using namespace std;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 720;

int main(int argv, char* args[]){
	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;
	SDL_Surface* the_image = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL has returned an error initializing\n");
		return 1;
	}
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		printf("SDL was unable to create the window\n");
		return 1;
	}
	screen_surface = SDL_GetWindowSurface(window);
	the_image = SDL_LoadBMP("images/player.bmp");
	SDL_BlitSurface(the_image, NULL, screen_surface, NULL);
	SDL_UpdateWindowSurface(window);



	SDL_Delay(2000);
	
	//clean up
	SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

