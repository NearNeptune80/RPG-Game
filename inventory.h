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
		void renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY);
		void renderDescription(SDL_Renderer* renderer);

		bool addItem(item newItem);
		void removeItem(int index);

		std::vector<item> storedItems;
};

