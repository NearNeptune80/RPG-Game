#pragma once

#include <iostream>
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

	int levelPoints;
	int atkLevel;
	int defLevel;
	int hpLevel;

	item equippedHelmet;
	item equippedChestplate;
	item equippedLeggings;
	item equippedBoots;
	item equippedWeapon;
	item equippedShield;
	item equippedAccessory1;
	item equippedAccessory2;
	item emptySlot = item();

	inventory playerInventory;

	player(std::string name);
	void calculateStats();
	void equipItem(item item);
	void unequipItem(item item);
	void addLevelPoint(int stat);
	int getLevelStats(int levelCat);
	item getEquippedItem(int mouseX, int mouseY);
};

