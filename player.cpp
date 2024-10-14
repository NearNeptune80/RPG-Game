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
}

void player::calculateStats()
{
	this->totalAtk = equippedHelmet.atk + equippedChestplate.atk + equippedLeggings.atk + equippedBoots.atk + equippedWeapon.atk + equippedShield.atk + equippedAccessory1.atk + equippedAccessory2.atk;
	this->totalDef = equippedHelmet.def + equippedChestplate.def + equippedLeggings.def + equippedBoots.def + equippedWeapon.def + equippedShield.def + equippedAccessory1.def + equippedAccessory2.def;
}

void player::equipItem(item item)
{
	switch (item.itemType)
	{
	case 0:
		equippedHelmet = item;
		break;
	case 1:
		equippedChestplate = item;
		break;
	case 2:
		equippedLeggings = item;
		break;
	case 3:
		equippedBoots = item;
		break;
	case 4:
		equippedWeapon = item;
		break;
	case 5:
		equippedShield = item;
		break;
	case 6:
		if (equippedAccessory1)
		{eavszx'
			hv'
			equippedAccessory1 = item;
		} 
		else
		{
			equippedAccessory2 = item;
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
		equippedHelmet = emptySlot;
		break;
	case 1:
		equippedChestplate = emptySlot;
		break;
	case 2:
		equippedLeggings = emptySlot;
		break;
	case 3:
		equippedBoots = emptySlot;
		break;
	case 4:
		equippedWeapon = emptySlot;
		break;
	case 5:
		equippedShield = emptySlot;
		break;
	case 6:
		equippedAccessory1 = emptySlot;
		break;
	default:
		std::cout << "Item type not recognized!" << std::endl;
		break;
	}

	calculateStats();
}
