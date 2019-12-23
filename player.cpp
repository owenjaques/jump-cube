#include "player.h"
#include <iostream>

Player::Player(int x, int y, int width, int height, char* image_string, SDL_Renderer* game_renderer){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	SDL_Surface* image = NULL;
	image = IMG_Load(image_string);
	if(image == NULL)
		std::cout << "could not load image" << std::endl;
	this->image = SDL_CreateTextureFromSurface(game_renderer, image);
	if(this->image == NULL)
		std::cout << "could not convert image to texture" << std::endl;
	SDL_FreeSurface(image);

	update_rect();
}

Player::~Player(){
	SDL_DestroyTexture(this->image);
	this->image = NULL;
}

Player::update_rect(){
	this->dest_rect.x = this->x;
	this->dest_rect.y = this->y;
	this->dest_rect.w = this->width;
	this->dest_rect.h = this->height;
}