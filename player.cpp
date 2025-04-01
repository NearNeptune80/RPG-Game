#include "player.h"


player::player(std::string name, SDL_Renderer* renderer)
	: playerInventory(renderer)
{
	this->playerName = name;
	this->hp = 100;
	this->maxHp = 100;
	this->level = 1;
	this->xp = 0;
	this->totalAtk = 0;
	this->totalDef = 0;
	this->gold = 0;

	this->levelPoints = 0;
	this->atkLevel = 0;
	this->defLevel = 0;
	this->hpLevel = 0;

	this->x = 0;
	this->y = 0;

	this->playerTextureMap = IMG_LoadTexture(renderer, textureMap.c_str());

}

void player::move(SDL_Renderer* renderer, int direction, int frame, float deltaTime, int mapHeight, int mapWidth)
{
	/*
	0 = up
	1 = down
	2 = left
	3 = right
	*/

	float speed = 300.0f; // Speed in pixels per second
	switch (direction)
	{
	case 0:
		y -= speed * deltaTime;
		if (y < 0)
		{
			y = 0;
			std::cout << "Player reached the top of the map!" << std::endl;
		}
		break;
	case 1:
		y += speed * deltaTime;
		if (y + FRAME_HEIGHT > mapHeight) 
		{
			y = mapHeight - FRAME_HEIGHT;
			std::cout << "Player reached the bottom of the map!" << std::endl;
		}
		break;
	case 2:
		x -= speed * deltaTime;
		if (x < 0)
		{
			x = 0;
			std::cout << "Player reached the left side of the map!" << std::endl;
		}
		break;
	case 3:
		x += speed * deltaTime;
		if (x + FRAME_WIDTH > mapWidth)
		{
			x = mapWidth - FRAME_WIDTH;
			std::cout << "Player reached the right side of the map!" << std::endl;
		}
		break;
	default:
		std::cout << "Direction not recognized!" << std::endl;
		break;
	}
	std::cout << "Player position: (" << x << ", " << y << ")" << std::endl;
}

void player::calculateStats()
{
	this->totalAtk = playerInventory.equippedItems[0].atk + playerInventory.equippedItems[1].atk + playerInventory.equippedItems[2].atk + playerInventory.equippedItems[3].atk + playerInventory.equippedItems[4].atk + playerInventory.equippedItems[5].atk + playerInventory.equippedItems[6].atk + playerInventory.equippedItems[7].atk + getLevelStats(0);
	this->totalDef = playerInventory.equippedItems[0].def + playerInventory.equippedItems[1].def + playerInventory.equippedItems[2].def + playerInventory.equippedItems[3].def + playerInventory.equippedItems[4].def + playerInventory.equippedItems[5].def + playerInventory.equippedItems[6].def + playerInventory.equippedItems[7].def + getLevelStats(1);
	this->maxHp = 100 + getLevelStats(2);
	std::cout << "Total attack: " << totalAtk << std::endl;
	std::cout << "Total defense: " << totalDef << std::endl;
	std::cout << "Max HP: " << maxHp << std::endl;
}

void player::equipItem(item item)
{
	int slot = item.itemType;
	if (slot < 0 || slot > 7)
	{
		std::cout << "Item type not recognized!" << std::endl;
		return;
	}

	if (item.lvl > level)
	{
		std::cout << "Item level is too high!" << std::endl;
	}
	else
	{
		if (playerInventory.equippedItems[slot].name != "")
		{
			unequipItem(playerInventory.equippedItems[slot]);
		}

		playerInventory.equippedItems[slot] = item;

		// Handle the special case for itemType 6
		if (slot == 6 && playerInventory.equippedItems[7].name == "")
		{
			playerInventory.equippedItems[7] = item;
		}
	}
	
	calculateStats();
}

void player::unequipItem(item unequippedItem)
{
	int slot = unequippedItem.itemType;
	if (slot < 0 || slot >= playerInventory.equippedItems.size())
	{
		std::cout << "Item type not recognized!" << std::endl;
		return;
	}

	// Add the unequipped item back to the inventory
	if (!playerInventory.addItem(playerInventory.equippedItems[slot]))
	{
		std::cout << "Failed to add item to inventory. Inventory might be full." << std::endl;
		return;
	}

	// Clear the equipment slot
	playerInventory.equippedItems[slot] = item();

	calculateStats();
}

void player::addLevelPoint(int stat)
{
	if (levelPoints > 0)
	{
		switch (stat)
		{
		case 0:
			atkLevel++;
			levelPoints--;
			break;
		case 1:
			defLevel++;
			levelPoints--;
			break;
		case 2:
			hpLevel++;
			levelPoints--;
			break;
		default:
			std::cout << "Stat not recognized!" << std::endl;
			break;
		}
	} 
	else
	{
		std::cout << "No level points available!" << std::endl;
	}
	calculateStats();
}

int player::getLevelStats(int levelCat)
{
	int levelTotal = 0;

	switch (levelCat)
	{
	case 0:
		levelTotal = atkLevel * 5;
		break;
	case 1:
		levelTotal = defLevel * 5;
		break;
	case 2:
		levelTotal = hpLevel * 5;
		break;
	default:
		std::cout << "Stat not recognized!" << std::endl;
		break;
	}

	return levelTotal;
}

void player::renderPlayer(SDL_Renderer* renderer, int direction, int frame, const camera& cam, int mapWidth, int mapHeight)
{
	/*
	0 = up
	1 = down
	2 = left
	3 = right
	*/

	// Calculate the source rectangle based on the direction and frame
	SDL_Rect srcRect;
	srcRect.x = frame * FRAME_WIDTH;
	srcRect.y = direction * FRAME_HEIGHT;
	srcRect.w = FRAME_WIDTH;
	srcRect.h = FRAME_HEIGHT;

	// Calculate the destination rectangle for rendering
	int renderX = static_cast<int>(x) - cam.x;
	int renderY = static_cast<int>(y) - cam.y;

	// Ensure the player is rendered within the camera view
	if (renderX < 0) renderX = 0;
	if (renderY < 0) renderY = 0;
	if (renderX + FRAME_WIDTH > cam.width) renderX = cam.width - FRAME_WIDTH;
	if (renderY + FRAME_HEIGHT > cam.height) renderY = cam.height - FRAME_HEIGHT;

	SDL_Rect destRect = { renderX, renderY, FRAME_WIDTH, FRAME_HEIGHT };

	// Render the player texture
	SDL_RenderCopy(renderer, playerTextureMap, &srcRect, &destRect);
}