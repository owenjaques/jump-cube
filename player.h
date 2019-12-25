#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <array>

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

class Player {
	public:
		//for rendering at different locations and different sizes
		SDL_Rect dest_rect;
		//the location of the player on the sprite sheet
		SDL_Rect src_rect;

		Player(int x, int y, int width, int height);

		void update(int frame, std::array<bool, 4> states);

	private:
		void get_direction(std::array<bool, 4> states);
		void move_right();
		void move_left();
};

#endif