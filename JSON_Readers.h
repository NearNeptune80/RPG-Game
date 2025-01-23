#pragma once

#include <vector>
#include <string>
#include "item.h"
#include "map.h"

map createMap(std::string mapLocation, std::vector<SDL_Texture*> tileTextures);
std::vector<item> readItems(const std::string& filePath, SDL_Renderer* renderer);
std::vector<SDL_Texture*> loadTileTextures(SDL_Renderer* renderer, SDL_Surface* tilesetSurface, int columns, int rows, int tileWidth, int tileHeight);