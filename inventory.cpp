#include "inventory.h"



inventory::inventory()
{
}

inventory::~inventory()
{
}

void inventory::renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font)
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

	SDL_Rect equipmentBoxes[8] = 
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
			renderDescription(renderer, mouseX, mouseY, font, equipmentBoxes[i].x, equipmentBoxes[i].y);
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
				renderDescription(renderer, mouseX, mouseY, font, i, j);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Normal color
			}
			SDL_RenderFillRect(renderer, &inventoryGrid);
		}
	}
}

void inventory::renderDescription(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, int i, int j)
{
	item descItem = getItem(mouseX, mouseY);
	if (descItem.name != "")
	{
		SDL_Rect descBox;
		SDL_Rect descInterior;

		if (i > 800)
		{
			descBox = { i - 214, j, 200, 200 };
			descInterior = { i - 204, j + 10, 180, 180 };
		}
		else
		{
			descBox = { i + 214, j, 200, 200 };
			descInterior = { i + 224, j + 10, 180, 180 };
		}
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
		SDL_RenderFillRect(renderer, &descBox);
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF);
		SDL_RenderFillRect(renderer, &descInterior);

		SDL_Color textColor = { 255, 255, 255 };
		SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, descItem.name.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		
		SDL_Rect descText = {
			descBox.x + (descBox.w - textSurface->w) / 2, // Center horizontally
			descBox.y + 10, // Center vertically
			textSurface->w,
			textSurface->h
		};

		SDL_RenderCopy(renderer, textTexture, NULL, &descText);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		int offsetY = descText.y + descText.h + 10; // Start below the item name
		auto renderStat = [&](const std::string& statName, int statValue) {
			if (statValue != 0)
			{
				std::string statText = statName + ": " + std::to_string(statValue);
				textSurface = TTF_RenderUTF8_Solid(font, statText.c_str(), textColor);
				textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

				SDL_Rect statRect = {
					descBox.x + 10, // Left margin
					offsetY,
					textSurface->w,
					textSurface->h
				};

				SDL_RenderCopy(renderer, textTexture, NULL, &statRect);
				SDL_FreeSurface(textSurface);
				SDL_DestroyTexture(textTexture);

				offsetY += statRect.h + 5; // Move down for the next stat
			}
		};

		renderStat("Attack", descItem.atk);
		renderStat("Defense", descItem.def);
		renderStat("Price", descItem.price);
		renderStat("Level", descItem.lvl);

		textSurface = TTF_RenderUTF8_Solid(font, descItem.description.c_str(), textColor);
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_Rect descRect = {
			descBox.x + 10, // Left margin
			offsetY,
			textSurface->w,
			textSurface->h
		};

		SDL_RenderCopy(renderer, textTexture, NULL, &descRect);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

	}
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

item inventory::getItem(int mouseX, int mouseY)
{
	for (int i = 1224; i < 1600; i += 94)
	{
		for (int j = 126; j <= 690; j += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			int index = (j - 126) / 94 * 4 + (i - 1224) / 94;
			if (mouseX >= inventoryGrid.x && mouseX <= inventoryGrid.x + inventoryGrid.w &&
				mouseY >= inventoryGrid.y && mouseY <= inventoryGrid.y + inventoryGrid.h && storedItems.size() > index)
			{
				std::cout << "Item found!" << storedItems[(j - 126) / 94 * 4 + (i - 1224) / 94].name << std::endl;
				return storedItems[(j - 126) / 94 * 4 + (i - 1224) / 94];
			}
		}
	}

	return item();
}
