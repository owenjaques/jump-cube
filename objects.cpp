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
	change_y();
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

	if(states[UP])
		jump();

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

void Player::jump(){
	//obviously will add stuff to see if in contact with stuff
	if(dest_rect.y == 300)
		velocity = -6;
}

//add function to see if in contact with stuff for this function and jump

void Player::change_y(){
	//obviously change things to test if they are contacting with anything
	for(int i = 0; i < abs((int)velocity); i++){
		if(velocity < 0)
			//if not hitting something from above
			dest_rect.y--;
		else
			if(dest_rect.y != 300)//if not hitting something from below
				dest_rect.y++;
	}

	//if not hitting something from below
	//this adds the gravity with a terminal velocity of 10 gravity
	if(dest_rect.y != 300){
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

Brick::Brick(int x, int y, int width, int height):Object(x, y, width, height){
	src_rect.x = 32;
	src_rect.y = 32;
	src_rect.h = 16;
	src_rect.w = 16;
}