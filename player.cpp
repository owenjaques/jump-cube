#include "player.h"
#include <iostream>

Player::Player(int x, int y, int width, int height){
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.h = 32;
	src_rect.w = 32;

	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = width;
	dest_rect.h = height;
}

void Player::update(int frame, std::array<bool, 4> states){
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
	get_direction(states);
}

void Player::get_direction(std::array<bool, 4> states){
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
	if(direction == LEFT)
		move_left();

	former_direction = direction;
	former_states = states;
}

void Player::move_left(){
	dest_rect.x -= 2;
}

void Player::move_right(){
	dest_rect.x += 2;
}