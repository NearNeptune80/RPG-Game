#include "tile.h"

tile::tile(int x, int y, bool solid, int tileType, std::vector<SDL_Texture*> tileTextures, std::optional<std::string> actionData, std::optional<int> destX, std::optional<int> destY)
{
	tileData.pos.x = x;
	tileData.pos.y = y;
	tileData.solid = solid;
	tileData.tileType = tileType;
	tileData.actionData = actionData.value_or("");
	tileData.destination.x = destX.value_or(0);
	tileData.destination.y = destY.value_or(0);
	this->tileTextures = tileTextures;
}

tile::~tile()
{
}
