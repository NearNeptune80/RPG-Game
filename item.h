#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class item
{
	public:

		item();
		item(int atk, int def, int price, const std::string& imagelocation, int itemType, const std::string& description, const std::string& name, int lvl, SDL_Renderer* renderer);
		~item();

		std::string getItemTypeName();
		
		int atk = 0;
		int def = 0;
		int price = 0;
		std::string imagelocation = "";
		std::shared_ptr<SDL_Texture> texture = nullptr;
		int itemType = -1;
		std::string description = "";
		std::string name = "";
		int lvl = 0;
};

