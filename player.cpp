#include "player.h"
#include <iostream>

Player::Player(int x, int y, int width, int height, char* image_string, SDL_Surface* screen_surface){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	SDL_Surface* image = NULL;
	image = SDL_LoadBMP(image_string);
	if(image == NULL)
		std::cout << "could not load image\n";
	this->image = SDL_ConvertSurface(image, screen_surface->format, 0);
	if(this->image == NULL)
		std::cout << "could not convert image\n";
	SDL_FreeSurface(image);

	update_rect();
}

Player::~Player(){
	SDL_FreeSurface(this->image);
}

Player::update_rect(){
	this->stretch_rect.x = this->x;
	this->stretch_rect.y = this->y;
	this->stretch_rect.w = this->width;
	this->stretch_rect.h = this->height;
}