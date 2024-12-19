#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include "SDL_Funcs.h"
#include "tile.h"
#include "JSON_Readers.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "shopManager.h"

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 896;
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILESET_COLUMNS = 10;
const int TILESET_ROWS = 2;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* tileset = IMG_Load("./Images/tileset.png");
std::string fontLocation = "./Roboto/Roboto-Black.ttf";


int main(int argc, char* args[]) {

	if (!tileset)
	{
		std::cout << "Tileset could not be loaded! SDL_Error: " << SDL_GetError() << std::endl;
	}
	
	std::srand(time(0));
	shopManager& shopManager = shopManager::getInstance();
	

    player player1("Jack");
	std::vector<item> itemList = readItems("./items.json");

	shopManager.randomizeShopInventory("shop1", itemList, 1);

	for (int i = 0; i < itemList.size(); i++)
	{
		std::cout << itemList[i].name << std::endl;
	}
	
	if (tileset == NULL) {
		std::cout << "Tileset could not be loaded! SDL_Error: " << SDL_GetError() << std::endl;
	}
	
	std::vector<mapInfo> mapIndex = readMapIndex("./Maps/allMaps.json");

	init(window, screenSurface, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	TTF_Font* font = TTF_OpenFont(fontLocation.c_str(), 20);
	TTF_Font* invInfoFont = TTF_OpenFont(fontLocation.c_str(), 27);
	TTF_Font* shopTitleFont = TTF_OpenFont(fontLocation.c_str(), 27);

	if (!font)
	{
		std::cout << "Font could not be loaded! SDL_Error: " << TTF_GetError() << std::endl;
	}

	std::vector<SDL_Texture*> tileTextures = loadTileTextures(renderer, tileset, TILESET_ROWS, TILESET_COLUMNS, TILE_WIDTH, TILE_HEIGHT);

	std::vector<mapData> map = readMapData("./Maps/startArea.json");
	std::vector<tile> tiles = createTiles(map);
	
	bool buttonHeld = false;
	bool exitInv = true;
	bool exitShop = true;
	bool close = false;
	int mouseX, mouseY;
	SDL_Event e;

	player1.levelPoints = 10;

	int num = 0;

	while (!close) {

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				close = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_w:
					exitShop = !exitShop;
					break;
				case SDLK_s:
					break;
				case SDLK_a:
					break;
				case SDLK_d:
					break;
				case SDLK_TAB:
					exitInv = !exitInv;
					break;
				case SDLK_e:
					//Interacting
					break;
				case SDLK_q:
					//Attacking

					player1.playerInventory.addItem(itemList[num]);
					num++;
					if (num >= itemList.size())
					{
						num = 0;
					}

					break;
				case SDLK_SPACE:
					//Blocking
					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				if (!exitInv)
				{
					SDL_GetMouseState(&mouseX, &mouseY);
					player1.playerInventory.getItem(mouseX, mouseY);
				}
				
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				buttonHeld = true;
				if (!exitInv)
				{
					if (player1.playerInventory.thingHovered(mouseX, mouseY) != -1)
					{
						player1.addLevelPoint(player1.playerInventory.thingHovered(mouseX, mouseY));
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				buttonHeld = false;
				auto draggedItem = player1.playerInventory.dragAndDrop(mouseX, mouseY, renderer, buttonHeld);
				if (draggedItem) {
					// Handle equipping the item
					player1.equipItem(*draggedItem);
				}
				if (!exitInv)
				{
					player1.playerInventory.getItem(mouseX, mouseY);
				}
			}
		}
		
		if (!exitInv)
		{
			player1.playerInventory.renderInventory(renderer, mouseX, mouseY, shopTitleFont, invInfoFont, player1.playerName, player1.level, player1.atkLevel, player1.defLevel, player1.hpLevel);
		}
		if (!exitShop)
		{
			shopManager.renderShopInventory(renderer, mouseX, mouseY, font, "First Shop");
		}
		
		SDL_GetMouseState(&mouseX, &mouseY);
		player1.playerInventory.dragAndDrop(mouseX, mouseY, renderer, buttonHeld);
		
		//Update screen
		SDL_RenderPresent(renderer);
	}
	
	quit(window, renderer);
	return 0;
}