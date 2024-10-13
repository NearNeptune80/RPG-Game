#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "item.h"

class inventory
{
	public:
		inventory();
		~inventory();
		void renderInventory(SDL_Renderer* renderer);
		void renderDescription(SDL_Renderer* renderer);

	private:
		std::vector<item> storedItems;
		std::vector<item> equippedItems;
		std::string playerName;
		int playerLevel;
};

