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
