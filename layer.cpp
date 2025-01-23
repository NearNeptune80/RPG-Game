#include "layer.h"

layer::layer(int width, int height)
{
	layerWidth = width;
	layerHeight = height;
}

layer::~layer()
{
}

void layer::addTile(tile newTile)
{
	tiles.push_back(newTile);
}
