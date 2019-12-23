#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>

//make this extend some object class later on

class Player {
	public:
		SDL_Texture* image;
		//for blitting at different locations and different sizes
		SDL_Rect dest_rect;
		int x;
		int y;
		int width;
		int height;
		Player(int x, int y, int width, int height, char* image_string, SDL_Renderer* game_renderer);
		~Player();
		update_rect();
		
};

#endif