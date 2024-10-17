#include "player.h"


player::player(std::string name)
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
}

void player::calculateStats()
{
	this->totalAtk = playerInventory.equippedItems[0].atk + playerInventory.equippedItems[1].atk + playerInventory.equippedItems[2].atk + playerInventory.equippedItems[3].atk + playerInventory.equippedItems[4].atk + playerInventory.equippedItems[5].atk + playerInventory.equippedItems[6].atk + playerInventory.equippedItems[7].atk + getLevelStats(0);
	this->totalDef = playerInventory.equippedItems[0].def + playerInventory.equippedItems[1].def + playerInventory.equippedItems[2].def + playerInventory.equippedItems[3].def + playerInventory.equippedItems[4].def + playerInventory.equippedItems[5].def + playerInventory.equippedItems[6].def + playerInventory.equippedItems[7].def + getLevelStats(1);
	this->maxHp = 100 + getLevelStats(2);
}

void player::equipItem(item item)
{
	switch (item.itemType)
	{
	case 0:
		if (playerInventory.equippedItems[0].name == "")
		{
			playerInventory.equippedItems[0] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[0]);
			playerInventory.equippedItems[0] = item;
		}
		break;
	case 1:
		if (playerInventory.equippedItems[1].name == "")
		{
			playerInventory.equippedItems[1] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[1]);
			playerInventory.equippedItems[1] = item;
		}
		break;
	case 2:
		if (playerInventory.equippedItems[2].name == "")
		{
			playerInventory.equippedItems[2] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[2]);
			playerInventory.equippedItems[2] = item;
		}
		break;
	case 3:
		if (playerInventory.equippedItems[3].name == "")
		{
			playerInventory.equippedItems[3] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[3]);
			playerInventory.equippedItems[3] = item;
		}
		break;
	case 4:
		if (playerInventory.equippedItems[4].name == "")
		{
			playerInventory.equippedItems[4] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[4]);
			playerInventory.equippedItems[4] = item;
		}
		break;
	case 5:
		if (playerInventory.equippedItems[5].name == "")
		{
			playerInventory.equippedItems[5] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[5]);
			playerInventory.equippedItems[5] = item;
		}
		break;
	case 6:
		if (playerInventory.equippedItems[6].name == "")
		{
			playerInventory.equippedItems[6] = item;
		} 
		else if (playerInventory.equippedItems[7].name == "")
		{
			playerInventory.equippedItems[7] = item;
		}
		else
		{
			unequipItem(playerInventory.equippedItems[6]);
			playerInventory.equippedItems[6] = item;
		}
		break;
	default:
		std::cout << "Item type not recognized!" << std::endl;
		break;
	}
	
	calculateStats();
}

void player::unequipItem(item item)
{
	switch (item.itemType)
	{
	case 0:
		playerInventory.addItem(playerInventory.equippedItems[0]);
		playerInventory.equippedItems[0] = item;
		break;
	case 1:
		playerInventory.addItem(playerInventory.equippedItems[1]);
		playerInventory.equippedItems[0] = item;
		break;
	case 2:
		playerInventory.addItem(playerInventory.equippedItems[2]);
		playerInventory.equippedItems[0] = item;
		break;
	case 3:
		playerInventory.addItem(playerInventory.equippedItems[3]);
		playerInventory.equippedItems[0] = item;
		break;
	case 4:
		playerInventory.addItem(playerInventory.equippedItems[4]);
		playerInventory.equippedItems[0] = item;
		break;
	case 5:
		playerInventory.addItem(playerInventory.equippedItems[5]);
		playerInventory.equippedItems[0] = item;
		break;
	case 6:
		playerInventory.addItem(playerInventory.equippedItems[6]);
		playerInventory.equippedItems[0] = item;
		break;
	default:
		std::cout << "Item type not recognized!" << std::endl;
		break;
	}

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
