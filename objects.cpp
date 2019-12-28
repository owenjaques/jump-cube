#include "objects.h"
#include <iostream>
#include <cmath>

Object::Object(int x, int y, int width, int height){
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = width;
	dest_rect.h = height;
}

Player::Player(int x, int y, int width, int height):Object(x, y, width, height){
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.h = 32;
	src_rect.w = 32;
	velocity = 0;
}

void Player::update(int frame, std::array<bool, 4> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	int direction = get_direction(states, map);
	change_y(map);
	//finds which frame it is at
	switch(frame / 4){
		case 0:
			src_rect.x = 0;
			break;
		case 1:
			src_rect.x = 32;
			break;
		case 2:
			src_rect.x = 64;
			break;
		case 3:
			src_rect.x = 96;
			break;
	}
	//finds which animation to display order is important here
	src_rect.y = 0;
	if(direction == RIGHT)
		src_rect.y = 96;
	else if(direction == LEFT)
		src_rect.y = 64;
	if(!is_colliding(DOWN, map))
		src_rect.y = 128;
	if(velocity == TERMINAL_VELOCITY)
		src_rect.y = 160;	
}

int Player::get_direction(std::array<bool, 4> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	//determines which way to go depending on the last key(s) pressed for smooth playing
	static int former_direction = -1;
	static std::array<bool, 4> former_states = {false, false, false, false};
	int direction = -1;
	if(states[RIGHT] && states[LEFT]){
		if(former_states[RIGHT] && former_states[LEFT])
			direction = former_direction;
		else if(former_direction == RIGHT)
			direction = LEFT;
		else 
			direction = RIGHT;
	}
	else if(states[RIGHT])
		direction = RIGHT;
	else if(states[LEFT])
		direction = LEFT;
	
	if(direction == RIGHT)
		move_right(map);
	else if(direction == LEFT)
		move_left(map);

	former_direction = direction;
	former_states = states;

	if(states[UP])
		if(jump(map))
			direction = UP;

	if(states[DOWN])
		if(drop(map))
			direction = DOWN;

	return direction;
}

void Player::move_left(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	if(velocity < TERMINAL_VELOCITY && !is_colliding(LEFT, map) && dest_rect.x >= 0)
		dest_rect.x -= 2;
}

void Player::move_right(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	if(velocity < TERMINAL_VELOCITY && !is_colliding(RIGHT, map) && dest_rect.x < SCREEN_WIDTH - dest_rect.w)
		dest_rect.x += 2;
}

bool Player::jump(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	if(is_colliding(DOWN, map)){
		velocity = -6;
		return true;
	}
	return false;
}

//change the == BRICK to is in something to hit function when there are more than bricks
bool Player::is_colliding(int direction, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	switch(direction){
		case UP:
			if((dest_rect.y - TILE_SIZE) % TILE_SIZE == 0 && (map[(dest_rect.y - TILE_SIZE) / TILE_SIZE][dest_rect.x / TILE_SIZE] == BRICK || map[(dest_rect.y - TILE_SIZE) / TILE_SIZE][(dest_rect.x / TILE_SIZE) + 1] == BRICK || (dest_rect.x % TILE_SIZE != 0 && map[(dest_rect.y - TILE_SIZE) / TILE_SIZE][(dest_rect.x / TILE_SIZE) + 2] == BRICK))){
				velocity = 0;
				return true;
			}
			break;
		case DOWN:
			if((dest_rect.y + dest_rect.h) % TILE_SIZE == 0 && (map[(dest_rect.y + dest_rect.h) / TILE_SIZE][dest_rect.x / TILE_SIZE] == BRICK || map[(dest_rect.y + dest_rect.h) / TILE_SIZE][(dest_rect.x / TILE_SIZE) + 1] == BRICK || (dest_rect.x % TILE_SIZE != 0 && map[(dest_rect.y + dest_rect.h) / TILE_SIZE][(dest_rect.x / TILE_SIZE) + 2] == BRICK)))
				return true;
			break;
		case LEFT:
			if(dest_rect.x % TILE_SIZE == 0 && (map[dest_rect.y / TILE_SIZE][(dest_rect.x / TILE_SIZE) - 1] == BRICK || map[(dest_rect.y / TILE_SIZE) + 1][(dest_rect.x / TILE_SIZE) - 1] == BRICK || (dest_rect.y % TILE_SIZE != 0 && map[(dest_rect.y / TILE_SIZE) + 2][(dest_rect.x / TILE_SIZE) - 1] == BRICK)))
				return true;
			break;
		case RIGHT:
			if(dest_rect.x % TILE_SIZE == 0 && (map[dest_rect.y / TILE_SIZE][(dest_rect.x / TILE_SIZE) + 2] == BRICK || map[(dest_rect.y / TILE_SIZE) + 1][(dest_rect.x / TILE_SIZE) + 2] == BRICK || (dest_rect.y % TILE_SIZE != 0 && map[(dest_rect.y / TILE_SIZE) + 2][(dest_rect.x / TILE_SIZE) + 2] == BRICK)))
				return true;
			break;
	}
	return false;
}

void Player::change_y(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	//obviously change things to test if they are contacting with anything
	for(int i = 0; i < abs((int)velocity); i++){
		if(velocity < 0){
			if(!is_colliding(UP, map))//if not hitting something from above
				dest_rect.y--;
		}
		else {
			if(!is_colliding(DOWN, map))//if not hitting something from below
				dest_rect.y++;
		}
	}

	//if not hitting something from below
	//this adds the gravity with a terminal velocity of 10 gravity
	if(!is_colliding(DOWN, map)){
		if(velocity < TERMINAL_VELOCITY - 0.5)
			velocity += 0.2;
	}
	else
		velocity = 0;	
}

bool Player::drop(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	if(!is_colliding(DOWN, map)){
		velocity = TERMINAL_VELOCITY;
		return true;
	}
	return false;
}

void Player::render(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet){
	SDL_RenderCopy(game_renderer, sprite_sheet, &src_rect, &dest_rect);
}

Cloud::Cloud(int x, int y, int width, int height):Object(x, y, width, height){
	src_rect.x = 64;
	src_rect.y = 32;
	src_rect.h = 32;
	src_rect.w = 64;
}

void Cloud::move(int direction){
	int move_speed = 1;
	if(direction == LEFT){
		if(dest_rect.x < -dest_rect.w){
			dest_rect.x = SCREEN_WIDTH + dest_rect.w;
			dest_rect.y = rand() % SCREEN_HEIGHT;
		}
		else
			dest_rect.x -= move_speed;
	}
	if(dest_rect.x > SCREEN_WIDTH + dest_rect.w){
		dest_rect.x = -dest_rect.w;
		dest_rect.y = rand() % SCREEN_HEIGHT;
	}
	else
		dest_rect.x += move_speed;
}

Clouds::Clouds(int max_val, int min_val){
	//randomly generates a list of clouds
	srand(time(NULL));
	int num_clouds = rand() % (max_val - min_val + 1) + min_val;
	for(int i = 0; i < num_clouds; i++)
		clouds.push_back(new Cloud(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 64, 32));
}

void Clouds::update(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet, int direction){
	for(std::list<Cloud*>::iterator it = clouds.begin(); it != clouds.end(); it++){
		(*it)->move(direction);
		SDL_RenderCopy(game_renderer, sprite_sheet, &((*it)->src_rect), &((*it)->dest_rect));
	}
}

Clouds::~Clouds(){
	clouds.clear();
}

Bullet::Bullet(int x, int y, int width, int height, int direction):Object(x, y, width, height){
	src_rect.x = 32;
	src_rect.y = 48;
	src_rect.h = 8;
	src_rect.w = 8;

	speed = 8;

	if(direction == RIGHT)
		speed *= -1;
}

void Bullet::update(SDL_Renderer* game_renderer, SDL_Texture* sprite_sheet){
	dest_rect.x += speed;
	SDL_RenderCopy(game_renderer, sprite_sheet, &src_rect, &dest_rect);
}