#pragma once

#include <vector>
#include "tile.h"

class layer
{
	public:

	layer(int width, int height);
	~layer();

	int layerWidth;
	int layerHeight;
	std::vector<tile> tiles;

	void addTile(tile newTile);
};

