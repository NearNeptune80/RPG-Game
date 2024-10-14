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
		if (equippedHelmet.name == "")
		{
			equippedHelmet = item;
		}
		else
		{
			unequipItem(equippedHelmet);
			equippedHelmet = item;
		}
		break;
	case 1:
		if (equippedChestplate.name == "")
		{
			equippedChestplate = item;
		}
		else
		{
			unequipItem(equippedChestplate);
			equippedChestplate = item;
		}
		break;
	case 2:
		if (equippedLeggings.name == "")
		{
			equippedLeggings = item;
		}
		else
		{
			unequipItem(equippedLeggings);
			equippedLeggings = item;
		}
		break;
	case 3:
		if (equippedBoots.name == "")
		{
			equippedBoots = item;
		}
		else
		{
			unequipItem(equippedBoots);
			equippedBoots = item;
		}
		break;
	case 4:
		if (equippedWeapon.name == "")
		{
			equippedWeapon = item;
		}
		else
		{
			unequipItem(equippedWeapon);
			equippedWeapon = item;
		}
		break;
	case 5:
		if (equippedShield.name == "")
		{
			equippedShield = item;
		}
		else
		{
			unequipItem(equippedShield);
			equippedShield = item;
		}
		break;
	case 6:
		if (equippedAccessory1.name == "")
		{
			equippedAccessory1 = item;
		} 
		else if (equippedAccessory2.name == "")
		{
			equippedAccessory2 = item;
		}
		else
		{
			unequipItem(equippedAccessory1);
			equippedAccessory1 = item;
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
		playerInventory.addItem(equippedHelmet);
		equippedHelmet = emptySlot;
		break;
	case 1:
		playerInventory.addItem(equippedChestplate);
		equippedChestplate = emptySlot;
		break;
	case 2:
		playerInventory.addItem(equippedLeggings);
		equippedLeggings = emptySlot;
		break;
	case 3:
		playerInventory.addItem(equippedBoots);
		equippedBoots = emptySlot;
		break;
	case 4:
		playerInventory.addItem(equippedWeapon);
		equippedWeapon = emptySlot;
		break;
	case 5:
		playerInventory.addItem(equippedShield);
		equippedShield = emptySlot;
		break;
	case 6:
		playerInventory.addItem(equippedAccessory1);
		equippedAccessory1 = emptySlot;
		break;
	default:
		std::cout << "Item type not recognized!" << std::endl;
		break;
	}

	calculateStats();
}
