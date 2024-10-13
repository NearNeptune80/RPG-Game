#include "item.h"

item::item()
{
}

item::item(int atk, int def, int price, std::string imagelocation, int itemType, std::string description, std::string name, int lvl)
{
	this->atk = atk;
	this->def = def;
	this->price = price;
	this->imagelocation = imagelocation;
	this->itemType = itemType;
	this->description = description;
	this->name = name;
	this->lvl = lvl;
}

item::~item()
{
}

std::string item::getItemTypeName()
{
	switch (itemType)
	{
	case 0:
		return "Helmet";
		break;
	case 1:
		return "Chestplate";
		break;
	case 2:
		return "Leggings";
		break;
	case 3:
		return "Boots";
		break;
	case 4:
		return "Weapon";
		break;
	case 5:
		return "Shield";
		break;
	case 6:
		return "Accessory";
		break;
	case 7:
		return "Consumable";
		break;
	default:
		return "Error";
		break;
	}
}