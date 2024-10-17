#pragma once

#include <iostream>
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
		void renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font);
		void renderDescription(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, int i, int j);

		bool addItem(item newItem);
		void removeItem(int index);

		item getItem(int mouseX, int mouseY);

		std::vector<item> equippedItems;
		std::vector<item> storedItems;
};

