#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "item.h"

class inventory
{
	public:

		inventory();
		~inventory();
		void renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, TTF_Font* invInfoFont, std::string playerName, int playerLvl, int atkLvl, int defLvl, int hpLvl);
		void renderDescription(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, int i, int j);
		void renderLvlUpBoxes(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, int previousBoxY, int previousBoxH, int atkLvl, int defLvl, int hpLvl);
		int thingHovered(int mouseX, int mouseY);

		std::optional<item> dragAndDrop(int mouseX, int mouseY, SDL_Renderer* renderer, bool buttonHeld);
		bool addItem(item newItem);
		void removeItem(int index);

		item getItem(int mouseX, int mouseY);

		std::vector<item> equippedItems;
		std::vector<item> storedItems;
};

