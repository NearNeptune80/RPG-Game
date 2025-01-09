#include "inventory.h"

const int MAX_INVENTORY_SIZE = 28;

inventory::inventory(SDL_Renderer* renderer)
{
	// Initialize equippedItems with default items
	for (int i = 0; i < 8; i++)
	{
		equippedItems.push_back(item(0, 0, 0, "", -1, "", "", 0, renderer));
	}

	// Initialize storedItems with default items
	for (int i = 0; i < MAX_INVENTORY_SIZE; i++)
	{
		storedItems.push_back(item());
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
		// Draw the equipment slot background first
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

		// Render the item image if it exists
		if (!equippedItems[i].imagelocation.empty())
		{
			if (equippedItems[i].texture != NULL)
			{
				// Define the destination rectangle for the item image with padding
				SDL_Rect itemRect = {
					equipmentBoxes[i].x + 5, // Slight padding inside the slot
					equipmentBoxes[i].y + 5,
					equipmentBoxes[i].w - 10, // Adjust width and height for padding
					equipmentBoxes[i].h - 10
				};

				// Render the item texture
				SDL_RenderCopy(renderer, equippedItems[i].texture.get(), NULL, &itemRect);
			}
			else
			{
				std::cout << "Could not load equipment item image! SDL_Error: " << IMG_GetError() << std::endl;
			}
		}
	}


	int index = 0;

	for (int j = 126; j <= 690; j += 94) // Rows
	{
		for (int i = 1224; i < 1600; i += 94) // Columns
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

			if (index < storedItems.size())
			{
				item& currentItem = storedItems[index];
				if (currentItem.texture)
				{
					SDL_Rect itemRect = {
						inventoryGrid.x + 5,
						inventoryGrid.y + 5,
						inventoryGrid.w - 10,
						inventoryGrid.h - 10
					};
					SDL_RenderCopy(renderer, currentItem.texture.get(), NULL, &itemRect);
				}
			}
			index++;
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
	static std::string sourceSlotType;
	static int sourceIndex = -1;

	if (buttonHeld && !isDragging)
	{
		auto [slotType, index] = getSlotTypeAndIndex(mouseX, mouseY);
		if (!slotType.empty() && index >= 0)
		{
			if (slotType == "inventory" && index < storedItems.size())
			{
				if (!storedItems[index].name.empty())
				{
					draggedItem = storedItems[index];
					isDragging = true;
					sourceSlotType = slotType;
					sourceIndex = index;
					// Clear the source slot visually
					storedItems[sourceIndex] = item();
				}
			}
			else if (slotType == "equipment" && index < equippedItems.size())
			{
				if (!equippedItems[index].name.empty())
				{
					draggedItem = equippedItems[index];
					isDragging = true;
					sourceSlotType = slotType;
					sourceIndex = index;
					// Clear the source slot visually
					equippedItems[sourceIndex] = item();
				}
			}
		}
	}
	else if (!buttonHeld && isDragging)
	{
		isDragging = false;
		auto [targetSlotType, targetIndex] = getSlotTypeAndIndex(mouseX, mouseY);

		bool dropSuccess = false;

		if (sourceSlotType == "inventory")
		{
			if (targetSlotType == "inventory" && targetIndex < storedItems.size())
			{
				// Swap items within inventory using explicit assignments
				item temp = storedItems[targetIndex];
				storedItems[targetIndex] = draggedItem;
				storedItems[sourceIndex] = temp;
				dropSuccess = true;
			}
			else if (targetSlotType == "equipment" && targetIndex < equippedItems.size())
			{
				if (isValidEquipmentSlot(draggedItem, targetIndex))
				{
					if (equippedItems[targetIndex].name.empty())
					{
						// Equip the item
						equippedItems[targetIndex] = draggedItem;
						equipmentChanged = true;
						dropSuccess = true;
					}
					else
					{
						// **Swap Only If Same Type** using explicit assignments
						if (draggedItem.itemType == equippedItems[targetIndex].itemType)
						{
							item temp = equippedItems[targetIndex];
							equippedItems[targetIndex] = draggedItem;
							storedItems[sourceIndex] = temp;
							equipmentChanged = true;
							dropSuccess = true;
						}
						else
						{
							// Cannot swap, do nothing
						}
					}
				}
				else
				{
					// Invalid slot for this item, do nothing
				}
			}
			else
			{
				// Dropped outside valid slots, do nothing
			}

			if (!dropSuccess)
			{
				// Return dragged item to source slot
				storedItems[sourceIndex] = draggedItem;
			}
		}
		else if (sourceSlotType == "equipment")
		{
			if (targetSlotType == "inventory" && targetIndex < storedItems.size())
			{
				if (storedItems[targetIndex].name.empty())
				{
					// Move item to inventory
					storedItems[targetIndex] = draggedItem;
					equipmentChanged = true;
					dropSuccess = true;
				}
				else
				{
					// **Swap Only If Same Type** using explicit assignments
					if (draggedItem.itemType == storedItems[targetIndex].itemType)
					{
						item temp = storedItems[targetIndex];
						storedItems[targetIndex] = draggedItem;
						equippedItems[sourceIndex] = temp;
						equipmentChanged = true;
						dropSuccess = true;
					}
					else
					{
						// Cannot swap, do nothing
					}
				}
			}
			else if (targetSlotType == "equipment" && targetIndex < equippedItems.size())
			{
				if (isValidEquipmentSlot(draggedItem, targetIndex))
				{
					if (equippedItems[targetIndex].name.empty())
					{
						// Place item into the empty equipment slot
						equippedItems[targetIndex] = draggedItem;
						equipmentChanged = true;
						dropSuccess = true;
					}
					else
					{
						// **Swap Only If Same Type** using explicit assignments
						if (draggedItem.itemType == equippedItems[targetIndex].itemType)
						{
							item temp = equippedItems[targetIndex];
							equippedItems[targetIndex] = draggedItem;
							equippedItems[sourceIndex] = temp;
							equipmentChanged = true;
							dropSuccess = true;
						}
						else
						{
							// Cannot swap, do nothing
						}
					}
				}
				else
				{
					// Invalid slot for this item, do nothing
				}
			}
			else
			{
				// Dropped outside valid slots, do nothing
			}

			if (!dropSuccess)
			{
				// Return dragged item to source slot
				equippedItems[sourceIndex] = draggedItem;
			}
		}

		// Clear draggedItem
		draggedItem = item();
		sourceSlotType.clear();
		sourceIndex = -1;
	}

	// Render the dragged item if any
	if (isDragging && draggedItem.texture)
	{
		SDL_Rect itemRect = { mouseX - 40, mouseY - 40, 80, 80 };
		SDL_RenderCopy(renderer, draggedItem.texture.get(), NULL, &itemRect);
	}

	return std::nullopt;
}


bool inventory::addItem(const item& newItem)
{
	for (auto& itm : storedItems)
	{
		if (itm.name.empty())
		{
			itm = newItem;
			return true;
		}
	}
	std::cout << "Inventory is full!" << std::endl;
	return false;
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
		int index = 0;

		for (int j = 126; j <= 690; j += 94)
		{
			for (int i = 1224; i < 1600; i += 94)
			{
				SDL_Rect inventoryGrid = { i, j, 80, 80 };
				if (mouseX >= inventoryGrid.x && mouseX <= inventoryGrid.x + inventoryGrid.w &&
					mouseY >= inventoryGrid.y && mouseY <= inventoryGrid.y + inventoryGrid.h &&
					index < storedItems.size())
				{
					return storedItems[index];
				}
				index++;
			}
		}
	}
	

	return item();
}

std::pair<std::string, int> inventory::getSlotTypeAndIndex(int mouseX, int mouseY)
{
	// Equipment slots
	int boxSize = 80;
	int padding = 14;
	int totalWidth = 3 * boxSize + 2 * padding; // Total width of the equipment grid
	int startX = (390 - totalWidth) / 2;        // Center the grid
	int startY = 220;

	SDL_Rect equipmentBoxes[8] =
	{
		{startX + 1 * (boxSize + padding), startY, boxSize, boxSize},                       // Helmet (0)
		{startX + 1 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Chestplate (1)
		{startX + 1 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Leggings (2)
		{startX + 1 * (boxSize + padding), startY + 3 * (boxSize + padding), boxSize, boxSize}, // Boots (3)
		{startX + 2 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Weapon (4)
		{startX + 0 * (boxSize + padding), startY + 2 * (boxSize + padding), boxSize, boxSize}, // Shield (5)
		{startX + 0 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}, // Accessory1 (6)
		{startX + 2 * (boxSize + padding), startY + 1 * (boxSize + padding), boxSize, boxSize}  // Accessory2 (7)
	};

	for (int i = 0; i < 8; ++i)
	{
		if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + equipmentBoxes[i].w &&
			mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + equipmentBoxes[i].h)
		{
			return { "equipment", i };
		}
	}

	// Inventory slots
	int index = 0;
	for (int j = 126; j <= 690; j += 94)
	{
		for (int i = 1224; i < 1600; i += 94)
		{
			SDL_Rect inventoryGrid = { i, j, 80, 80 };
			if (mouseX >= inventoryGrid.x && mouseX <= inventoryGrid.x + inventoryGrid.w &&
				mouseY >= inventoryGrid.y && mouseY <= inventoryGrid.y + inventoryGrid.h)
			{
				return { "inventory", index };
			}
			index++;
		}
	}

	return { "", -1 }; // Not over any slot
}

// inventory.cpp

bool inventory::isValidEquipmentSlot(const item& itm, int slotIndex)
{
	if (itm.itemType == slotIndex)
	{
		return true;
	}
	// Handle accessory slots (e.g., slot indices 6 and 7 for accessories)
	if (itm.itemType == 6 && (slotIndex == 6 || slotIndex == 7))
	{
		return true;
	}
	return false;
}

bool inventory::isInventoryFull()
{
	for (const auto& itm : storedItems)
	{
		if (itm.name.empty())
		{
			return false; // Found an empty slot
		}
	}
	return true; // No empty slots found
}