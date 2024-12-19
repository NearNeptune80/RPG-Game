#include "inventory.h"



inventory::inventory()
{
	for (int i = 0; i < 8; i++)
	{
		equippedItems.push_back(item());
	}
}

inventory::~inventory()
{
}

void inventory::renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, TTF_Font* invInfoFont, std::string playerName, int playerLvl, int atkLvl, int defLvl, int hpLvl)
{
	SDL_Rect equipmentContainer = { 0, 112, 390, 672 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &equipmentContainer);

	SDL_Rect charaInfoBox = { 14, 126, 362, 80 };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
	SDL_RenderFillRect(renderer, &charaInfoBox);

	SDL_Color nameTextColor = { 255, 255, 255 };
	SDL_Surface* nameTextSurface = TTF_RenderText_Blended(invInfoFont, playerName.c_str(), nameTextColor);
	SDL_Texture* nameTextTexture = SDL_CreateTextureFromSurface(renderer, nameTextSurface);

	SDL_Rect nameText = {
			charaInfoBox.x + 10, // Center horizontally
			charaInfoBox.y + 10, // Center vertically
			nameTextSurface->w,
			nameTextSurface->h
	};

	SDL_RenderCopy(renderer, nameTextTexture, NULL, &nameText);
	SDL_FreeSurface(nameTextSurface);
	SDL_DestroyTexture(nameTextTexture);

	std::string levelText = "Level " + std::to_string(playerLvl);

	SDL_Color lvlTextColor = { 255, 255, 255 };
    SDL_Surface* lvlTextSurface = TTF_RenderText_Blended(invInfoFont, levelText.c_str(), lvlTextColor);
	SDL_Texture* lvlTextTexture = SDL_CreateTextureFromSurface(renderer, lvlTextSurface);

	SDL_Rect lvlText = {
			charaInfoBox.x + 10, // Center horizontally
			charaInfoBox.y + charaInfoBox.h - 37, // Center vertically
			lvlTextSurface->w,
			lvlTextSurface->h
	};

	SDL_RenderCopy(renderer, lvlTextTexture, NULL, &lvlText);
	SDL_FreeSurface(lvlTextSurface);
	SDL_DestroyTexture(lvlTextTexture);

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
		{startX + 1 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Chestplate
		{startX + 1 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Leggings
		{startX + 1 * (boxSize + padding), startY + 3 * (boxSize + padding), boxSize, boxSize},  // Boots
		{startX + 2 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Weapon
		{startX + 0 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Shield
		{startX + 0 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Accessory1
		{startX + 2 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize} // Accessory2
	};

	for (int i = 0; i < 8; ++i)
	{
		SDL_Surface* itemImage = IMG_Load(equippedItems[i].imagelocation.c_str());
		if (itemImage == NULL)
		{
			std::cout << "Could not load image! SDL_Error: " << SDL_GetError() << i << std::endl;
		}
		SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, itemImage);
		SDL_Rect itemRect = { equipmentBoxes[i].x, equipmentBoxes[i].y, equipmentBoxes[i].w, equipmentBoxes[i].h };

		// Render the item texture
		SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);

		// Free the surface and destroy the texture
		SDL_FreeSurface(itemImage);
		SDL_DestroyTexture(itemTexture);

		if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + equipmentBoxes[i].w &&
			mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + equipmentBoxes[i].h)
		{
			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100); // Highlight color
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100); // Normal color
		}
		SDL_RenderFillRect(renderer, &equipmentBoxes[i]);
	}

	for (int i = 1224; i < 1600; i += 94)
	{
		for (int j = 126; j <= 690; j += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			/*
			SDL_Surface* itemImage = IMG_Load(storedItems[(j - 126)].imagelocation.c_str());
			SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, itemImage);
			SDL_Rect itemRect = { inventoryGrid.x, inventoryGrid.y, inventoryGrid.w, inventoryGrid.h };
			*/

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

	renderLvlUpBoxes(renderer, font, mouseX, mouseY, equipmentBoxes[3].y, equipmentBoxes[3].h, atkLvl, defLvl, hpLvl);

	// Render description after all boxes
	for (int i = 0; i < 8; ++i)
	{
		if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + equipmentBoxes[i].w &&
			mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + equipmentBoxes[i].h)
		{
			renderDescription(renderer, mouseX, mouseY, font, equipmentBoxes[i].x, equipmentBoxes[i].y);
		}
	}

	for (int i = 1224; i < 1600; i += 94)
	{
		for (int j = 126; j <= 690; j += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			if (mouseX >= inventoryGrid.x && mouseX <= inventoryGrid.x + inventoryGrid.w &&
				mouseY >= inventoryGrid.y && mouseY <= inventoryGrid.y + inventoryGrid.h)
			{
				renderDescription(renderer, mouseX, mouseY, font, i, j);
			}
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
			descBox = { i - 214, j, 200, 300 };
			descInterior = { i - 204, j + 10, 180, 280 };
		}
		else
		{
			descBox = { i + 94, j, 200, 200 };
			descInterior = { i + 104, j + 10, 180, 180 };
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
		SDL_RenderFillRect(renderer, &descBox);
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF);
		SDL_RenderFillRect(renderer, &descInterior);

		SDL_Color textColor = { 255, 255, 255 };
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, descItem.name.c_str(), textColor, 170);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		
		SDL_Rect descText = {
			descInterior.x + (descInterior.w - textSurface->w) / 2, // Center horizontally
			descInterior.y + 5, // Center vertically
			textSurface->w,
			textSurface->h
		};

		SDL_RenderCopy(renderer, textTexture, NULL, &descText);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		int offsetY = descText.y + descText.h + 5; // Start below the item name

		auto renderStat = [&](const std::string& statName, int statValue) {
			if (statValue != 0)
			{
				std::string statText = statName + ": " + std::to_string(statValue);
				textSurface = TTF_RenderText_Blended_Wrapped(font, statText.c_str(), textColor, 170);
				textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

				SDL_Rect statRect = {
					descInterior.x + 5, // Left margin
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

		textSurface = TTF_RenderText_Blended_Wrapped(font, descItem.description.c_str(), textColor, 170);
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_Rect descRect = {
			descInterior.x + 5, // Left margin
			offsetY,
			textSurface->w,
			textSurface->h
		};

		SDL_RenderCopy(renderer, textTexture, NULL, &descRect);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

	}
}

void inventory::renderLvlUpBoxes(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, int previousBoxY, int previousBoxH, int atkLvl, int defLvl, int hpLvl)
{
	int nPreviousBoxY = previousBoxY + previousBoxH;
	int boxWidth = 362;
	for (int i = 0; i < 3; i++)
	{
		SDL_Rect lvlUpBox = { 14, nPreviousBoxY + 35, 362, 40 };
		SDL_Rect lvlUpIcon = { 19, nPreviousBoxY + 40, 30, 30 };

		if (mouseX >= lvlUpIcon.x && mouseX <= lvlUpIcon.x + lvlUpIcon.w &&
			mouseY >= lvlUpIcon.y && mouseY <= lvlUpIcon.y + lvlUpIcon.h)
		{
			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF); // Normal color
			SDL_RenderFillRect(renderer, &lvlUpBox);
			SDL_SetRenderDrawColor(renderer, 200, 200, 0, 200); // Highlight color
			SDL_RenderFillRect(renderer, &lvlUpIcon);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Normal color
			SDL_RenderFillRect(renderer, &lvlUpBox);
			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 200); // Normal color
			SDL_RenderFillRect(renderer, &lvlUpIcon);
		}

		nPreviousBoxY += 54;
	}

	std::vector<std::string> statNames = { "Attack", "Defense", "Health" };

	previousBoxY += previousBoxH;

	for (int i = 0; i < 3; i++)
	{
		SDL_Color textColor = { 255, 255, 255 };
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, statNames[i].c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_Surface* statValueSurface = TTF_RenderText_Blended(font, std::to_string(i == 0 ? atkLvl : i == 1 ? defLvl : hpLvl).c_str(), textColor);
		SDL_Texture* statValueTexture = SDL_CreateTextureFromSurface(renderer, statValueSurface);

		SDL_Rect textRect = {
			54, // Center horizontally
			previousBoxY + 40, // Center vertically
			textSurface->w + 10,
			textSurface->h + 5
		};

		SDL_Rect statValueRect = {
			boxWidth - 20,
			previousBoxY + 40, // Center vertically
			statValueSurface->w + 5,
			statValueSurface->h + 5
		};

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		SDL_RenderCopy(renderer, statValueTexture, NULL, &statValueRect);
		SDL_FreeSurface(statValueSurface);
		SDL_DestroyTexture(statValueTexture);

		previousBoxY += 54;
	}
}

int inventory::thingHovered(int mouseX, int mouseY)
{
	// Check if the mouse is over a stat box
	int previousBoxY = 502; // Adjust this value based on your layout
	int previousBoxH = 80;  // Adjust this value based on your layout
	int nPreviousBoxY = previousBoxY + previousBoxH;

	for (int i = 0; i < 3; i++)
	{
		SDL_Rect lvlUpIcon = { 19, nPreviousBoxY + 40, 30, 30 };
		if (mouseX >= lvlUpIcon.x && mouseX <= lvlUpIcon.x + lvlUpIcon.w &&
			mouseY >= lvlUpIcon.y && mouseY <= lvlUpIcon.y + lvlUpIcon.h)
		{
			return i;
		}

		nPreviousBoxY += 54;
	}
	return -1;
}

std::optional<item> inventory::dragAndDrop(int mouseX, int mouseY, SDL_Renderer* renderer, bool buttonHeld)
{
	static item draggedItem;
	static bool isDragging = false;
	static SDL_Texture* itemTexture = nullptr;

	if (buttonHeld && !isDragging)
	{
		draggedItem = getItem(mouseX, mouseY);
		if (draggedItem.name != "")
		{
			isDragging = true;
			SDL_Surface* itemImage = IMG_Load(draggedItem.imagelocation.c_str());
			if (itemImage == NULL)
			{
				std::cout << "Could not load draggedItem image! SDL_Error: " << SDL_GetError() << std::endl;
			}
			itemTexture = SDL_CreateTextureFromSurface(renderer, itemImage);
			SDL_FreeSurface(itemImage);
		}
	}
	else if (!buttonHeld && isDragging)
	{
		isDragging = false;
		SDL_DestroyTexture(itemTexture);
		itemTexture = nullptr;
		return draggedItem; // Return the dragged item when the button is released
	}

	if (isDragging && itemTexture
	{
		SDL_Rect itemRect = { mouseX - 40, mouseY - 40, 80, 80 };
		// Render the item texture
		SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);
	}

	return std::nullopt; // Return an empty optional if no item is being dragged
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
	if (mouseX < 800) {
		for (int i = 0; i < 8; ++i)
		{
			int boxSize = 80;
			int padding = 14;
			int totalWidth = 3 * boxSize + 2 * padding; // Total width of the equipment grid
			int startX = (390 - totalWidth) / 2; // Center the grid
			int startY = 220;

			SDL_Rect equipmentBoxes[8] =
			{
				{startX + 1 * (boxSize + padding), startY, boxSize, boxSize}, // Helmet
				{startX + 1 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Chestplate
				{startX + 1 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Leggings
				{startX + 1 * (boxSize + padding), startY + 3 * (boxSize + padding), boxSize, boxSize},  // Boots
				{startX + 2 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Weapon
				{startX + 0 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Shield
				{startX + 0 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Accessory1
				{startX + 2 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize} // Accessory2
			};

			if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + equipmentBoxes[i].w &&
				mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + equipmentBoxes[i].h && equippedItems.size() > i)
			{
				if (i < equippedItems.size()) 
				{
					return equippedItems[i];
				}
			}
		}
	}
	else {
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
	}
	

	return item();
}
