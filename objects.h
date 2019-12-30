#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>
#include <SDL_image.h>
#include <array>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

class Object;
class Bullet;
class Player;
class Cloud;
class Clouds;
class Enemies;

class Object {
	public:
		//for rendering at different locations and different sizes
		SDL_Rect dest_rect;
		//the location of the player on the sprite sheet
		SDL_Rect src_rect;
		
		Object(int x, int y, int width, int height);
};

class Bullet: public Object {
	public:
		Bullet(int x, int y, int width, int height, int direction);
		void render(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet);
		void update();
		bool is_colliding_with_brick(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool is_colliding_with_player(Player* player);

	private:
		int speed;
};

class Player: public Object {
	public:
		Player(int x, int y, int width, int height);
		bool update(int frame, std::array<bool, 6> states, Enemies enemies, Player* player, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void render(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet);
		std::list<Bullet*> bullets;
		bool is_shot(Enemies enemies, Player* player);

	private:
		const int TERMINAL_VELOCITY = 10;
		const int MAX_BULLETS = 4;
		double velocity;
		int get_direction(std::array<bool, 6> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_right(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void move_left(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool jump(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool drop(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void change_y(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool is_colliding(int direction, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void fire(int DIRECTION);
		void delete_bullets(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		bool is_dead(Enemies enemies, Player* player);
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

class Enemies {
	public:
		void render(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet);
		void update(int frame, Player* player, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]);
		void add_enemy(int x, int y);
		std::list<Bullet*> get_bullets();
		Enemies(Uint8 red, Uint8 green, Uint8 blue);
	private:
		Uint8 red;
		Uint8 green;
		Uint8 blue;
		std::list<Player*> enemies;
};

#endif