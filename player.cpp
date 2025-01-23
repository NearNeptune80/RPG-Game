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

void player::move(SDL_Renderer* renderer, int direction, int frame)
{
	/*
	0 = up
	1 = down
	2 = left
	3 = right
	*/
	switch (direction)
	{
	case 0:
		y -= 5;
		break;
	case 1:
		y += 5;
		break;
	case 2:
		x -= 5;
		break;
	case 3:
		x += 5;
		break;
	default:
		std::cout << "Direction not recognized!" << std::endl;
		break;
	}
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

void player::renderPlayer(SDL_Renderer* renderer, int direction, int frame)
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

	// Destination rectangle where the player will be rendered
	SDL_Rect destRect = { x - FRAME_WIDTH / 2, y - FRAME_HEIGHT / 2, FRAME_WIDTH, FRAME_HEIGHT };

	// Render the player texture
	SDL_RenderCopy(renderer, playerTextureMap, &srcRect, &destRect);
}
