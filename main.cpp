#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_Funcs.h"
#include "tile.h"
#include "JSON_Readers.h"
#include "inventory.h"
#include "item.h"

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
TTF_Font* font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);


int main(int argc, char* args[]) {

	inventory playerInventory;
	std::vector<item> itemList = readItems("./items.json");

	for (int i = 0; i < itemList.size(); i++)
	{
		std::cout << itemList[i].name << std::endl;
	}
	
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
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		playerInventory.renderInventory(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}
	
	quit(window, renderer);
	return 0;
}