#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_Funcs.h"
#include "tile.h"
#include "JSON_Readers.h"

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 896;
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILESET_COLUMNS = 10;
const int TILESET_ROWS = 2;

SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* tileset = IMG_Load("./Images/tileset.png");


int main(int argc, char* args[]) {
	
	if (tileset == NULL) {
		std::cout << "Tileset could not be loaded! SDL_Error: " << SDL_GetError() << std::endl;
	}
	
	std::vector<mapInfo> mapIndex = readMapIndex("./Maps/allMaps.json");

	init(window, screenSurface, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	std::vector<SDL_Texture*> tileTextures = loadTileTextures(renderer, tileset, TILESET_ROWS, TILESET_COLUMNS, TILE_WIDTH, TILE_HEIGHT);

	std::vector<mapData> map = readMapData("./Maps/startArea.json");
	std::vector<tile> tiles = createTiles(map);
	bool close = false;
	SDL_Event e;

	while (!close) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				close = true;
			}
		}
		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		//Render red filled quad
		SDL_Rect fillRect = { 0, 98, 390, 672 };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &fillRect);

		

		for (int i = 14; i < 390; i += 94)
		{
			for (int j = 112; j <= 676; j += 94)
			{
				SDL_Rect tileRect = { i, j, 80, 80 };
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &tileRect);
			}
		}
		//Update screen
		SDL_RenderPresent(renderer);
	}
	
	quit(window, renderer);
	return 0;
}