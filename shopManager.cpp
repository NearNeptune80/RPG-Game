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

void shopManager::renderShopInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, const std::string& shopId)
{
	SDL_Rect titleContainer = { 600, 14, 400, 100 };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
	SDL_RenderFillRect(renderer, &titleContainer);

	SDL_Rect titleBox = { 614, 28, 372, 72 };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
	SDL_RenderFillRect(renderer, &titleBox);
	
	SDL_Color titleTextColour = { 255, 255, 255 };
	SDL_Surface* titleTextSurface = TTF_RenderText_Blended(font, shopId.c_str(), titleTextColour);
	SDL_Texture* titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);

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

	SDL_FreeSurface(titleTextSurface);
	SDL_DestroyTexture(titleTextTexture);

	for (int i = 128; i <= 640; i += 128)
	{
		SDL_Rect itemContainer = { 1186, i, 400, 114 };
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
		SDL_RenderFillRect(renderer, &itemContainer);

		SDL_Rect itemBox = { 1200, i + 14, 86, 86 };
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
		SDL_RenderFillRect(renderer, &itemBox);

		SDL_Rect nameBox = { 1300, i + 14, 272, 86 };
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
		SDL_RenderFillRect(renderer, &nameBox);
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
