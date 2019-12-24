#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>

//add states for different animations

class Player {
	public:
		//for rendering at different locations and different sizes
		SDL_Rect dest_rect;
		//the location of the player on the sprite sheet
		SDL_Rect src_rect;

		Player(int x, int y, int width, int height);

		void update_animation(int frame);
		void move_right();
		void move_left();
};

#endif