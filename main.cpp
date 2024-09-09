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




int main(int argc, char* args[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* tileset = IMG_Load("./Images/tileset.png");
	if (tileset == NULL) {
		std::cout << "Tileset could not be loaded! SDL_Error: " << SDL_GetError() << std::endl;
	}
	
	std::vector<mapInfo> mapIndex = readMapIndex("./Maps/allMaps.json");

	init(window, screenSurface, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	std::vector<SDL_Texture*> tileTextures = loadTileTextures(renderer, tileset, TILESET_ROWS, TILESET_COLUMNS, TILE_WIDTH, TILE_HEIGHT);

	std::vector<mapData> map = readMapData("./Maps/startArea.json");
	std::vector<tile> tiles = createTiles(map);
	
	
	SDL_Delay(2000);

	quit(window, renderer);
	return 0;
}