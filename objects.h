#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>
#include <SDL_image.h>
#include <array>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

class Object {
	public:
		//for rendering at different locations and different sizes
		SDL_Rect dest_rect;
		//the location of the player on the sprite sheet
		SDL_Rect src_rect;
		
		Object(int x, int y, int width, int height);
		bool is_colliding(int direction, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
};

class Bullet: public Object {
	public:
		Bullet(int x, int y, int width, int height, int direction);
		void update(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet);

	private:
		int speed;
};

class Player: public Object {
	public:
		Player(int x, int y, int width, int height);
		void update(int frame, std::array<bool, 6> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void render(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet);

	private:
		const int TERMINAL_VELOCITY = 10;
		const int MAX_BULLETS = 4;
		double velocity;
		std::list<Bullet*> bullets;
		int get_direction(std::array<bool, 6> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_right(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_left(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool jump(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool drop(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void change_y(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void fire(int DIRECTION);
		void delete_bullets(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
};

class Cloud: public Object {
	public:
		Cloud(int x, int y, int width, int height);
		void move(int direction);
};

class Clouds {
	public:
		std::list<Cloud*> clouds;
		Clouds(int max_val, int min_val);
		~Clouds();
		void update(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet, int direction);
};

#endif