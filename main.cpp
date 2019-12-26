#include <iostream>
#include <array>
#include <list> 
#include <SDL.h>
#include <SDL_image.h>

#include "constants.h"
#include "objects.h"

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* game_renderer = NULL;
SDL_Texture* sprite_sheet = NULL;

int init(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL has returned an error initializing" << endl;
		return 0;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "SDL was unable to initialize imaging" << endl;
		return 0;
	}
	window = SDL_CreateWindow( "Jump Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		cout << "SDL was unable to create the window" << endl;
		return 0;
	}
	game_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!game_renderer){
		cout << "SDL was unable to create the game renderer" << endl;
		return 0;
	}
	SDL_SetRenderDrawColor(game_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//loads the sprite sheet as a texture
	SDL_Surface* image = NULL;
	image = IMG_Load("images/spritesheet.png");
	if(image == NULL){
		cout << "could not load image" << endl;
		return 0;
	}
	sprite_sheet = SDL_CreateTextureFromSurface(game_renderer, image);
	if(sprite_sheet == NULL){
		cout << "could not convert image to texture" << endl;
		return 0;
	}
	SDL_FreeSurface(image);

	return 1;
}

void clean_up(){
	SDL_DestroyRenderer(game_renderer);
	game_renderer = NULL;
	SDL_DestroyTexture(sprite_sheet);
	sprite_sheet = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
    SDL_Quit();
}

int main(int argv, char* args[]){
	if(!init())
		return 1;

	Player* player = new Player(20, 304, 32, 32);

	//remove all this eventually to read levels from files
	int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE];
	for(int i = 0; i < SCREEN_HEIGHT/TILE_SIZE; i++)
		for(int j = 0; j < SCREEN_WIDTH/TILE_SIZE; j++)
			map[i][j] = SKY;

	for(int i = 0; i < SCREEN_WIDTH/16; i++){
		map[336/TILE_SIZE][i] = BRICK;
		map[(336-64)/16][i] = BRICK;
	}

	map[(336-16)/TILE_SIZE][10] = BRICK;
	map[(336-32)/TILE_SIZE][10] = BRICK;
	map[(336-48)/TILE_SIZE][10] = BRICK;
	map[(336-64)/TILE_SIZE][10] = BRICK;

	map[(336-64)/16][4] = SKY;
	map[(336-64)/16][5] = SKY;
	map[(336-64)/16][6] = SKY;

	map[(336-64)/16][12] = SKY;
	map[(336-64)/16][13] = SKY;
	map[(336-64)/16][14] = SKY;

	SDL_Rect sky_src_rect;
	sky_src_rect.x = 0;
	sky_src_rect.y = 32;
	sky_src_rect.w = 16;
	sky_src_rect.h = 16;

	SDL_Rect brick_src_rect;
	brick_src_rect.x = 32;
	brick_src_rect.y = 32;
	brick_src_rect.h = 16;
	brick_src_rect.w = 16;

	bool exit = false;
	SDL_Event e;
	int frame = 0;
	while(!exit){
		//finds out which keys have been pressed
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				exit = true;
		}
		//sees if the player is being moved at all
		const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
		array<bool, 4> states = {false, false, false, false};
		if(current_key_states[SDL_SCANCODE_UP])
			states[UP] = true;
		if(current_key_states[SDL_SCANCODE_DOWN])
			states[DOWN] = true;
		if(current_key_states[SDL_SCANCODE_LEFT])
			states[LEFT] = true;
		if(current_key_states[SDL_SCANCODE_RIGHT])
			states[RIGHT] = true;
		player->update(frame, states, map);

		SDL_RenderClear(game_renderer);

		//renders the map
		SDL_Rect dest_rect;
		SDL_Rect* src_rect = NULL;
		for(int i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++)
			for(int j = 0; j < SCREEN_WIDTH / TILE_SIZE; j++){
				//decided what to render
				if(map[i][j] == BRICK)
					src_rect = &brick_src_rect;
				else if(map[i][j] == SKY)
					src_rect = &sky_src_rect;

				dest_rect = (SDL_Rect){j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};

				SDL_RenderCopy(game_renderer, sprite_sheet, &(*src_rect), &(dest_rect));
			}

		//render player
		SDL_RenderCopy(game_renderer, sprite_sheet, &(player->src_rect), &(player->dest_rect));
		
		//update screen
		SDL_RenderPresent(game_renderer);

		frame = (frame / 4 == 4) ? 0 : frame + 1;
	}

	delete player;
	clean_up();
	return 0;
}

