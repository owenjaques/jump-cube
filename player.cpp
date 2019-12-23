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