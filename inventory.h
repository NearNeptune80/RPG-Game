#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "item.h"

class inventory
{
	public:

		inventory(SDL_Renderer* renderer);
		~inventory();
		void renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, TTF_Font* invInfoFont, const std::string& playerName, int playerLvl, int atkLvl, int defLvl, int hpLvl);
		void renderDescription(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, int i, int j);
		void renderLvlUpBoxes(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, int previousBoxY, int previousBoxH, int atkLvl, int defLvl, int hpLvl);
		int thingHovered(int mouseX, int mouseY);

		std::optional<item> dragAndDrop(int mouseX, int mouseY, SDL_Renderer* renderer, bool buttonHeld, int playerLvl);

		bool addItem(const item& newItem);
		void removeItem(int index);

		item getItem(int mouseX, int mouseY);

		std::pair<std::string, int> getSlotTypeAndIndex(int mouseX, int mouseY);

		bool isValidEquipmentSlot(const item& itm, int slotIndex);

		bool isInventoryFull();

		std::vector<item> equippedItems;
		std::vector<item> storedItems;
		bool equipmentChanged = false;

		bool isDragging = false;
		std::string sourceSlotType;
		int sourceIndex = -1;
		item draggedItem;

		std::vector<int> equipmentOrder = { 0, 6, 1, 7, 5, 2, 4, 3 };
};

