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

	inventory playerInventory;

	player(std::string name, SDL_Renderer* renderer);
	void calculateStats();
	void equipItem(item item);
	void unequipItem(item item);
	void addLevelPoint(int stat);
	int getLevelStats(int levelCat);
};

