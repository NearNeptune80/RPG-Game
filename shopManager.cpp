#include "shopManager.h"

shopManager& shopManager::getInstance()
{
	static shopManager shopManager;
	return shopManager;
}

void shopManager::randomizeShopInventory(const std::string& shopId, std::vector<item> itemList, int playerLvl)
{
	filterItems(itemList, playerLvl);
	generateRandomItems(shopInventories[shopId], allowedItems);
}

const std::vector<item>& shopManager::getShopInventory(const std::string& shopId)
{
	return shopInventories[shopId];
}

item shopManager::purchaseItem(item selectedItem, int& playerMoney, const std::string& shopId)
{
    // Check if the player has enough money to purchase the item
    if (playerMoney >= selectedItem.price)
    {
        // Deduct the price from the player's money
        playerMoney -= selectedItem.price;
        // Remove the purchased item from the shop inventory
        auto& shopInventory = shopInventories[shopId];
        auto it = std::find_if(shopInventory.begin(), shopInventory.end(), [&selectedItem](const item& i) {
            return i.name == selectedItem.name && i.price == selectedItem.price && i.lvl == selectedItem.lvl;
            });
        if (it != shopInventory.end())
        {
            shopInventory.erase(it);
        }
        // Return the purchased item
        return selectedItem;
    }
    else
    {
        std::cout << "You don't have enough money to purchase this item!" << std::endl;
        // Return an empty item if the player cannot afford the item
        return item();
    }
}

item shopManager::getHoveredItem(int mouseX, int mouseY, const std::string& shopId)
{
    const auto& inventory = shopInventories[shopId];
    int startY = 128;
    int spacingY = 128;
    int maxBoxes = 5;

    for (int i = 0; i < maxBoxes; ++i)
    {
        int yPos = startY + i * spacingY;
        SDL_Rect itemContainer = { 1186, yPos, 400, 114 };

        if (mouseX >= itemContainer.x && mouseX <= itemContainer.x + itemContainer.w &&
            mouseY >= itemContainer.y && mouseY <= itemContainer.y + itemContainer.h)
        {
            if (i < inventory.size())
            {
                return inventory[i];
            }
        }
    }
    return item(); // Return an empty item if no item is hovered
}

void shopManager::renderShopInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, const std::string& shopId)
{
    // Render Title Container
    SDL_Rect titleContainer = { 600, 14, 400, 100 };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
    SDL_RenderFillRect(renderer, &titleContainer);

    // Render Title Box
    SDL_Rect titleBox = { 614, 28, 372, 72 };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
    SDL_RenderFillRect(renderer, &titleBox);

    // Render Title Text
    SDL_Color titleTextColour = { 255, 255, 255 };
    SDL_Surface* titleTextSurface = TTF_RenderText_Blended(font, shopId.c_str(), titleTextColour);
    if (titleTextSurface)
    {
        SDL_Texture* titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
        if (titleTextTexture)
        {
            int textWidth = titleTextSurface->w;
            int textHeight = titleTextSurface->h;

            int scaleFactor = 2;
            int scaledWidth = textWidth * scaleFactor;
            int scaledHeight = textHeight * scaleFactor;

            SDL_Rect titleTextRect = {
                titleBox.x + (titleBox.w - scaledWidth) / 2,
                titleBox.y + (titleBox.h - scaledHeight) / 2,
                scaledWidth,
                scaledHeight
            };
            SDL_RenderCopy(renderer, titleTextTexture, NULL, &titleTextRect);
            SDL_DestroyTexture(titleTextTexture);
        }
        SDL_FreeSurface(titleTextSurface);
    }

    // Retrieve Shop Inventory
    const auto& inventory = shopInventories[shopId];

    // Define box positions
    int startY = 128;
    int spacingY = 128;
    int maxBoxes = 5; // Number of item boxes to render
    const int borderSize = 5; // Border size in pixels
    item hoveredItem; // To store the item being hovered
    bool isAnyHovered = false; // Flag to check if any item is hovered

    // Variables to store hovered item container position
    int hoveredItemContainerX = 0;
    int hoveredItemContainerY = 0;

    for (int i = 0; i < maxBoxes; ++i)
    {
        int yPos = startY + i * spacingY;

        // Define Item Container Rect
        SDL_Rect itemContainer = { 1186, yPos, 400, 114 };

        // Determine if the mouse is over the entire item container
        bool isHovered = (mouseX >= itemContainer.x && mouseX <= itemContainer.x + itemContainer.w &&
            mouseY >= itemContainer.y && mouseY <= itemContainer.y + itemContainer.h);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF); // Normal color
        SDL_RenderFillRect(renderer, &itemContainer);

        // Render Item Container with Highlight if Hovered
        if (isHovered)
        {
            SDL_SetRenderDrawColor(renderer, 150, 100, 100, 0xFF); // Semi-transparent Red
            SDL_RenderFillRect(renderer, &itemContainer);
        }
        
        // Render Item Box (for Image)
        SDL_Rect itemBox = { 1200, yPos + 14, 86, 86 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Fixed color
        SDL_RenderFillRect(renderer, &itemBox);

        // Render Name Box (for Item Name and Price)
        SDL_Rect nameBox = { 1300, yPos + 14, 272, 86 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Fixed color
        SDL_RenderFillRect(renderer, &nameBox);

        // Initialize nameHeight before using it
        int nameHeight = 0;

        // Check if there's an item for this box
        if (i < inventory.size())
        {
            const item& currentItem = inventory[i];

            // Render Item Texture if Available with Border
            if (currentItem.texture)
            {
                SDL_Rect textureRect = {
                    itemBox.x + borderSize,                     // x position with border
                    itemBox.y + borderSize,                     // y position with border
                    itemBox.w - 2 * borderSize,                 // width adjusted for borders
                    itemBox.h - 2 * borderSize                  // height adjusted for borders
                };
                SDL_RenderCopy(renderer, currentItem.texture.get(), NULL, &textureRect);
            }

            // Render Item Name
            if (!currentItem.name.empty())
            {
                SDL_Surface* nameSurface = TTF_RenderText_Blended(font, currentItem.name.c_str(), titleTextColour);
                if (nameSurface)
                {
                    SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                    if (nameTexture)
                    {
                        int nameWidth = nameSurface->w;
                        nameHeight = nameSurface->h; // Assign to the declared variable
                        SDL_Rect nameTextRect = {
                            nameBox.x + 5, // Padding from the left
                            nameBox.y + 5, // Top padding
                            nameWidth,
                            nameHeight
                        };
                        SDL_RenderCopy(renderer, nameTexture, NULL, &nameTextRect);
                        SDL_DestroyTexture(nameTexture);
                    }
                    SDL_FreeSurface(nameSurface);
                }
            }

            // Render Item Price
            if (currentItem.price > 0)
            {
                std::string priceText = "Price: " + std::to_string(currentItem.price);
                SDL_Surface* priceSurface = TTF_RenderText_Blended(font, priceText.c_str(), titleTextColour);
                if (priceSurface)
                {
                    SDL_Texture* priceTexture = SDL_CreateTextureFromSurface(renderer, priceSurface);
                    if (priceTexture)
                    {
                        int priceWidth = priceSurface->w;
                        int priceHeight = priceSurface->h;
                        SDL_Rect priceTextRect = {
                            nameBox.x + 5, // Padding from the left
                            nameBox.y + 5 + nameHeight + 5, // Below the name with spacing
                            priceWidth,
                            priceHeight
                        };
                        SDL_RenderCopy(renderer, priceTexture, NULL, &priceTextRect);
                        SDL_DestroyTexture(priceTexture);
                    }
                    SDL_FreeSurface(priceSurface);
                }
            }

            // If this box is hovered, store the current item for description
            if (isHovered)
            {
                hoveredItem = currentItem;
                isAnyHovered = true;
                hoveredItemContainerX = itemContainer.x;
                hoveredItemContainerY = itemContainer.y;
            }
        }
    }

    // Render Description Box if any item is hovered
    if (isAnyHovered && !hoveredItem.name.empty())
    {
        // Define description box dimensions (matching inventory.cpp)
        const int descWidth = 200;
        const int descHeight = 250;
        const int padding = 10; // Padding between descBox and itemContainer

        // Calculate description box position to snap to the edge of the hovered item container with padding
        int descX = hoveredItemContainerX - 200 - padding; // Positioned to the right of the itemContainer
        int descY = hoveredItemContainerY; // Align top with the hovered item container

        SDL_Rect descBox = { descX, descY, descWidth, descHeight };
        SDL_Rect descInterior = { descBox.x + 10, descBox.y + 10, descBox.w - 20, descBox.h - 20 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF); // Matching inventory.cpp
        SDL_RenderFillRect(renderer, &descBox);
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF); // Matching inventory.cpp
        SDL_RenderFillRect(renderer, &descInterior);

        SDL_Color textColor = { 255, 255, 255 };
        int offsetY = descInterior.y + 5;

        // Render Item Name
        SDL_Surface* nameSurface = TTF_RenderText_Blended(font, hoveredItem.name.c_str(), textColor);
        if (nameSurface)
        {
            SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
            if (nameTexture)
            {
                SDL_Rect textRect = { descInterior.x + 5, offsetY, nameSurface->w, nameSurface->h };
                SDL_RenderCopy(renderer, nameTexture, NULL, &textRect);
                SDL_DestroyTexture(nameTexture);
                offsetY += textRect.h + 5;
            }
            SDL_FreeSurface(nameSurface);
        }

        // Render Item Attributes
        auto renderAttribute = [&](const std::string& label, int value) {
            if (value != 0)
            {
                std::string text = label + ": " + std::to_string(value);
                SDL_Surface* attrSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
                if (attrSurface)
                {
                    SDL_Texture* attrTexture = SDL_CreateTextureFromSurface(renderer, attrSurface);
                    if (attrTexture)
                    {
                        SDL_Rect attrRect = { descInterior.x + 5, offsetY, attrSurface->w, attrSurface->h };
                        SDL_RenderCopy(renderer, attrTexture, NULL, &attrRect);
                        SDL_DestroyTexture(attrTexture);
                        offsetY += attrRect.h + 5;
                    }
                    SDL_FreeSurface(attrSurface);
                }
            }
            };

        renderAttribute("Attack", hoveredItem.atk);
        renderAttribute("Defense", hoveredItem.def);
        renderAttribute("Price", hoveredItem.price);
        renderAttribute("Level", hoveredItem.lvl);

        // Render Item Description
        if (!hoveredItem.description.empty())
        {
            SDL_Surface* descSurface = TTF_RenderText_Blended_Wrapped(font, hoveredItem.description.c_str(), textColor, descInterior.w - 10);
            if (descSurface)
            {
                SDL_Texture* descTexture = SDL_CreateTextureFromSurface(renderer, descSurface);
                if (descTexture)
                {
                    SDL_Rect descTextRect = { descInterior.x + 5, offsetY, descSurface->w, descSurface->h };
                    SDL_RenderCopy(renderer, descTexture, NULL, &descTextRect);
                    SDL_DestroyTexture(descTexture);
                }
                SDL_FreeSurface(descSurface);
            }
        }
    }
}



void shopManager::filterItems(const std::vector<item>& itemList, int playerLvl)
{
	allowedItems.clear();
	for (int i = 0; i < itemList.size(); i++)
	{
		if (itemList[i].lvl <= playerLvl)
		{
			allowedItems.push_back(itemList[i]);
		}
	}
}

void shopManager::generateRandomItems(std::vector<item>& shopItems, std::vector<item> allowedItems)
{
	shopItems.clear();
	for (int i = 0; i < 5; i++)
	{
		int randNum = rand() % allowedItems.size();

		shopItems.push_back(allowedItems[randNum]);
	}
}
