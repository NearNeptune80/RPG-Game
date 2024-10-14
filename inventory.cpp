#include "inventory.h"



inventory::inventory()
{
}

inventory::~inventory()
{
}

void inventory::renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY)
{
	SDL_Rect equipmentContainer = { 0, 112, 390, 672 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &equipmentContainer);

	SDL_Rect charaInfoBox = { 14, 126, 362, 80 };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
	SDL_RenderFillRect(renderer, &charaInfoBox);

	SDL_Rect inventoryContainer = { 1210, 112, 390, 672 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &inventoryContainer);

	// Equipment Boxes
	int boxSize = 80;
	int padding = 14;
	int totalWidth = 3 * boxSize + 2 * padding; // Total width of the equipment grid
	int startX = (equipmentContainer.w - totalWidth) / 2; // Center the grid
	int startY = 220;

	SDL_Rect equipmentBoxes[9] = 
	{
		{startX + 1 * (boxSize + padding), startY, boxSize, boxSize}, // Helmet
		{startX + 0 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Accessory1
		{startX + 1 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Chestplate
		{startX + 2 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Accessory2
		{startX + 0 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Shield
		{startX + 1 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Leggings
		{startX + 2 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Weapon
		{startX + 1 * (boxSize + padding), startY + 3 * (boxSize + padding), boxSize, boxSize}  // Boots
	};

	for (int i = 0; i < 8; ++i)
	{
		if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + equipmentBoxes[i].w &&
			mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + equipmentBoxes[i].h)
		{
			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF); // Highlight color
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Normal color
		}
		SDL_RenderFillRect(renderer, &equipmentBoxes[i]);
	}

	for (int i = 1224; i < 1600; i += 94)
	{
		for (int j = 126; j <= 690; j += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			if (mouseX >= inventoryGrid.x && mouseX <= inventoryGrid.x + inventoryGrid.w &&
				mouseY >= inventoryGrid.y && mouseY <= inventoryGrid.y + inventoryGrid.h)
			{
				SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF); // Highlight color
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Normal color
			}
			SDL_RenderFillRect(renderer, &inventoryGrid);
		}
	}
}

void inventory::renderDescription(SDL_Renderer* renderer)
{
}

bool inventory::addItem(item newItem)
{
	if (storedItems.size() < 28)
	{
		storedItems.push_back(newItem);
		return true;
	}
	else
	{
		std::cout << "Inventory is full!" << std::endl;
		return false;
	}
}

void inventory::removeItem(int index)
{
	storedItems.erase(storedItems.begin() + index);
}
