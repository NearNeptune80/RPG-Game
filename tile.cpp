#include "tile.h"
#include "JSON_Readers.h"
#include <string>
#include <iostream>
#include <vector>

tile::tile(int tileType, int solid, int trigger, std::optional<std::string> triggerData) {
	this->tileType = tileType;
	if (solid == 1) {
		this->solid = true;
	}
	else {
		this->solid = false;
	}
	this->trigger = trigger;
	this->triggerData = triggerData;
}

std::vector<tile> createTiles(std::vector<mapData> mapDataList) {
	std::vector<tile> tiles;
	for (auto tileData : mapDataList) {
		tile newTile(tileData.tileType, tileData.solid, tileData.trigger, tileData.triggerData);
		tiles.push_back(newTile);
	}
	return tiles;
}

std::vector<SDL_Texture*> loadTileTextures(SDL_Renderer* renderer, SDL_Surface* tileset, const int TILESET_ROWS, const int TILESET_COLUMNS, const int TILE_WIDTH, const int TILE_HEIGHT) {
	std::vector<SDL_Texture*> tileTextures;
	for (int i = 0; i < TILESET_ROWS; ++i) {
		for (int j = 0; j < TILESET_COLUMNS; ++j) {
			SDL_Rect tileRect;
			tileRect.x = j * TILE_WIDTH;
			tileRect.y = i * TILE_HEIGHT;
			tileRect.w = TILE_WIDTH;
			tileRect.h = TILE_HEIGHT;

			SDL_Surface* tileSurface = SDL_CreateRGBSurface(0, TILE_WIDTH, TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
			SDL_BlitSurface(tileset, &tileRect, tileSurface, NULL);
			SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
			SDL_FreeSurface(tileSurface);

			if (tileTexture == NULL) {
				std::cerr << "Unable to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
			}
			else {
				tileTextures.push_back(tileTexture);
			}
		}
	}
	return tileTextures;
}

void renderTiles(SDL_Renderer* renderer, const std::vector<tile>& tiles, const std::vector<SDL_Texture*>& tileTextures, int mapWidth, const int TILE_WIDTH, const int TILE_HEIGHT) {
	for (size_t i = 0; i < tiles.size(); ++i) {
		int tileType = tiles[i].tileType;
		int x = (i % mapWidth) * TILE_WIDTH;
		int y = (i / mapWidth) * TILE_HEIGHT;

		SDL_Rect destRect;
		destRect.x = x;
		destRect.y = y;
		destRect.w = TILE_WIDTH;
		destRect.h = TILE_HEIGHT;

		SDL_RenderCopy(renderer, tileTextures[tileType], NULL, &destRect);
	}
}