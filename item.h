#pragma once

#include <string>

class item
{
	public:

		item();
		item(int atk, int def, int price, std::string imagelocation, int itemType, std::string description, std::string name, int lvl);
		~item();

		std::string getItemTypeName();
		
		int atk = 0;
		int def = 0;
		int price = 0;
		std::string imagelocation = "";
		int itemType = -1;
		std::string description = "";
		std::string name = "";
		int lvl = 0;
};

