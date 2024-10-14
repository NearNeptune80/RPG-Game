#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "item.h"

class inventory
{
	public:
		inventory();
		~inventory();
		void renderInventory(SDL_Renderer* renderer);
		void renderDescription(SDL_Renderer* renderer);

		void addItem(item newItem);
		void removeItem(int index);

		void equipItem(int index);
		void unequipItem(int index);

	private:
		std::vector<item> storedItems;
		std::vector<item> equippedItems;
};

