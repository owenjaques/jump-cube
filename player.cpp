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

void Player::update_animation(int frame){
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
}

void Player::move_left(){
	dest_rect.x -= 2;
}

void Player::move_right(){
	dest_rect.x += 2;
}