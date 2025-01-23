#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include <optional>

class tile
{
	public:

	tile(int x, int y, bool solid, int tileType, std::vector<SDL_Texture*> tileTextures, std::optional<std::string> actionData, std::optional<int> destX, std::optional<int> destY);
	~tile();

	struct position
	{
		int x;
		int y;
	};

	struct data
	{
		/*
		Tile types:
		0 = standard tile
		1 = transport tile
		2 = shop tile
		*/
		int tileType;
		bool solid;
		std::string actionData;
		position destination;
		position pos;
	};

	std::vector<SDL_Texture*> tileTextures;
	data tileData;
};

