#pragma once

#include <string>

class item
{
	public:

		item();
		item(int atk, int def, int price, std::string imagelocation, int itemType, std::string description, std::string name, int lvl);
		~item();

		std::string getItemTypeName();
		
		int atk;
		int def;
		int price;
		std::string imagelocation;
		int itemType;
		std::string description;
		std::string name;
		int lvl;
};

