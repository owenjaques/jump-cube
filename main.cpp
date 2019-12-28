#include <iostream>
#include <array>
#include <fstream>
#include <list> 
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "objects.h"

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* game_renderer = NULL;
SDL_Texture* sprite_sheet = NULL;
int map[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE];

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

void read_map_from_file(string file_name){
	ifstream the_file;
	int c;
	int j = 0;
	int i = 0;
	the_file.open(file_name);
	while(the_file >> c){
		map[j][i] = c;
		i++;
		if(i == SCREEN_WIDTH / TILE_SIZE){
			i = 0;
			j++;
		}
	}
	the_file.close();


}

SDL_Rect rand_sky_tile(int i, int j){
	SDL_Rect sky_src_rect1;
	sky_src_rect1.x = 0;
	sky_src_rect1.y = 32;
	sky_src_rect1.w = 16;
	sky_src_rect1.h = 16;

	SDL_Rect sky_src_rect2;
	sky_src_rect2.x = 0;
	sky_src_rect2.y = 48;
	sky_src_rect2.w = 16;
	sky_src_rect2.h = 16;

	SDL_Rect sky_src_rect3;
	sky_src_rect3.x = 16;
	sky_src_rect3.y = 48;
	sky_src_rect3.w = 16;
	sky_src_rect3.h = 16;

	SDL_Rect sky_src_rect4;
	sky_src_rect4.x = 16;
	sky_src_rect4.y = 32;
	sky_src_rect4.w = 16;
	sky_src_rect4.h = 16;

	int x = (i*7 + j*11);
	cout << x;
	switch(x % 4){
		case 0:
			return sky_src_rect1;
		case 1:
			return sky_src_rect2;
		case 2:
			return sky_src_rect3;
		default:
			return sky_src_rect4;
	}
}

int main(int argv, char* args[]){
	if(!init())
		return 1;

	Player* player = new Player(20, 200, 32, 32);
	read_map_from_file("levels/level1.map");

	SDL_Rect brick_src_rect;
	brick_src_rect.x = 32;
	brick_src_rect.y = 32;
	brick_src_rect.h = 16;
	brick_src_rect.w = 16;

	//randomly generates a list of clouds
	srand(time(NULL));
	int num_clouds = rand() % 8 + 3;
	list<Cloud*> clouds;
	for(int i = 0; i < num_clouds; i++)
		clouds.push_back(new Cloud(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 64, 32));

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

		//sees if the player should die
		if(player->dest_rect.y > SCREEN_HEIGHT){
			delete player;
			player = new Player(20, 200, 32, 32);
		}

		SDL_RenderClear(game_renderer);

		//renders the map
		SDL_Rect dest_rect;
		SDL_Rect* src_rect = NULL;
		for(int i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++)
			for(int j = 0; j < SCREEN_WIDTH / TILE_SIZE; j++){
				//decided what to render
				if(map[i][j] == BRICK){
					src_rect = &brick_src_rect;
				}
				else if(map[i][j] == SKY){
					SDL_Rect sky_tile = rand_sky_tile(i, j);
					src_rect = &sky_tile;
				}

				dest_rect = (SDL_Rect){j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};

				SDL_RenderCopy(game_renderer, sprite_sheet, &(*src_rect), &(dest_rect));
			}

		//render player
		SDL_RenderCopy(game_renderer, sprite_sheet, &(player->src_rect), &(player->dest_rect));

		//render clouds
		for(list<Cloud*>::iterator it = clouds.begin(); it != clouds.end(); it++){
			(*it)->move(RIGHT);
			SDL_RenderCopy(game_renderer, sprite_sheet, &((*it)->src_rect), &((*it)->dest_rect));
		}
		
		//update screen
		SDL_RenderPresent(game_renderer);

		frame = (frame / 4 == 4) ? 0 : frame + 1;
	}

	delete player;
	clouds.clear();
	clean_up();
	return 0;
}

