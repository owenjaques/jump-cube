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
	get_direction(states, map);
	change_y(map);
}

void Player::get_direction(std::array<bool, 4> states, int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
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
		move_right();
	else if(direction == LEFT)
		move_left();

	former_direction = direction;
	former_states = states;

	if(states[UP])
		jump(map);

	if(states[DOWN])
		drop();
}

void Player::move_left(){
	if(velocity < TERMINAL_VELOCITY)
		dest_rect.x -= 2;
}

void Player::move_right(){
	if(velocity < TERMINAL_VELOCITY)
		dest_rect.x += 2;
}

void Player::jump(int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE]){
	//obviously will add stuff to see if in contact with stuff
	if(is_colliding(DOWN, map))
		velocity = -6;
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
			break;
		case RIGHT:
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
		if(velocity < TERMINAL_VELOCITY)
			velocity += 0.2;
	}
	else
		velocity = 0;	
}

void Player::drop(){
	if(dest_rect.y != 300)
		velocity = 10;
}