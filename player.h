#pragma once

#include <string>
#include "inventory.h"
#include "item.h"

class player
{
public:
	std::string playerName;
	int hp;
	int maxHp;
	int level;
	int xp;
	int totalAtk;
	int totalDef;
	int gold;

	item equippedHelmet;
	item equippedChestplate;
	item equippedLeggings;
	item equippedBoots;
	item equippedWeapon;
	item equippedShield;
	item equippedAccessory1;
	item equippedAccessory2;	

	inventory playerInventory;

	player(std::string name);
	void calculateStats();
};

