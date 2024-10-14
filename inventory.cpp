#include "inventory.h"



inventory::inventory()
{
}

inventory::~inventory()
{
}

void inventory::renderInventory(SDL_Renderer* renderer)
{
	SDL_Rect equipmentContainer = { 0, 98, 390, 672 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &equipmentContainer);

	SDL_Rect charaInfoBox = { 14, 112, 362, 80 };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
	SDL_RenderFillRect(renderer, &charaInfoBox);

	SDL_Rect inventoryContainer = { 1210, 98, 390, 672 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &inventoryContainer);

	for (int i = 14; i < 390; i += 94)
	{
		for (int j = 206; j <= 676; j += 94)
		{
			SDL_Rect equipmentGrid = { i, j, 80, 80 };
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
			SDL_RenderFillRect(renderer, &equipmentGrid);
		}
	}

	for (int i = 1224; i < 1600; i += 94)
	{
		for (int j = 112; j <= 676; j += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
			SDL_RenderFillRect(renderer, &inventoryGrid);
		}
	}
}

void inventory::renderDescription(SDL_Renderer* renderer)
{
}

void inventory::addItem(item newItem)
{
	storedItems.push_back(newItem);
}

void inventory::removeItem(int index)
{
	storedItems.erase(storedItems.begin() + index);
}
