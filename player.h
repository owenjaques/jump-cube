#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
//make this extend some object class later on

class Player {
	public:
		SDL_Surface* image;
		//for blitting at different locations and different sizes
		SDL_Rect stretch_rect;
		int x;
		int y;
		int width;
		int height;
		Player(int x, int y, int width, int height, char* image_string, SDL_Surface* screen_surface);
		~Player();
		update_rect();
		
};

#endif