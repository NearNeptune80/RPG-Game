#pragma once

#include "JSON_Readers.h"
#include <optional>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

class tile
{
	public:
		int tileType;
		bool solid;
		int trigger = -1;
		std::optional<std::string> triggerData;
		SDL_Texture* tileImage = NULL;

		tile(int tileType, int solid, int trigger, std::optional<std::string> triggerData);
		

};

std::vector<tile> createTiles(std::vector<mapData> mapDataList);
std::vector<SDL_Texture*> loadTileTextures(SDL_Renderer* renderer, SDL_Surface* tileset, const int TILESET_ROWS, const int TILESET_COLUMNS, const int TILE_WIDTH, const int TILE_HEIGHT);
void renderTiles(SDL_Renderer* renderer, const std::vector<tile>& tiles, const std::vector<SDL_Texture*>& tileTextures, int mapWidth, const int TILE_WIDTH, const int TILE_HEIGHT);