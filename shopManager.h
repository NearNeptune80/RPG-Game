#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "item.h"
#include <SDL.h>
#include <SDL_ttf.h>

class shopManager {
public:
    static shopManager& getInstance();

    void randomizeShopInventory(const std::string& shopId, std::vector<item> itemList, int playerLvl);
    const std::vector<item>& getShopInventory(const std::string& shopId);
    void renderShopInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, const std::string& shopId);

private:
    shopManager() = default;
    std::unordered_map<std::string, std::vector<item>> shopInventories;
    std::vector<item> allowedItems;
	void filterItems(const std::vector<item>& itemList, int playerLvl);
    void generateRandomItems(std::vector<item>& shopItems, std::vector<item> allowedItems);
};
