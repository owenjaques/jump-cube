#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>
#include <SDL_image.h>
#include <array>
#include "constants.h"

class Object {
	public:
		//for rendering at different locations and different sizes
		SDL_Rect dest_rect;
		//the location of the player on the sprite sheet
		SDL_Rect src_rect;
		
		Object(int x, int y, int width, int height);
};

class Player: public Object {
	public:
		Player(int x, int y, int width, int height);
		void update(int frame, std::array<bool, 4> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);

	private:
		const int TERMINAL_VELOCITY = 10;
		double velocity;
		void get_direction(std::array<bool, 4> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_right(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_left(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void jump(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void drop(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void change_y(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool is_colliding(int direction, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
};

#endif