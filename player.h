#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include "inventory.h"
#include "item.h"
#include "camera.h"

class player
{
public:
	std::string playerName;
	int hp;
	int maxHp;
	int level;
	int xp;
	int totalAtk;
	int totalDef;
	int gold;

	int levelPoints;
	int atkLevel;
	int defLevel;
	int hpLevel;

	float x;
	float y;

	inventory playerInventory;

	player(std::string name, SDL_Renderer* renderer);

	std::string textureMap = "./Images/playerTextureMap.png";
	SDL_Texture* playerTextureMap;
	const int FRAME_WIDTH = 48;
	const int FRAME_HEIGHT = 48;
	const int FRAMES_PER_DIRECTION = 4;

	void move(SDL_Renderer* renderer, int direction, int frame, float deltaTime, int mapWidth, int mapHeight);

	void calculateStats();
	void equipItem(item item);
	void unequipItem(item item);
	void addLevelPoint(int stat);
	int getLevelStats(int levelCat);
	void renderPlayer(SDL_Renderer* renderer, int direction, int frame, const camera& cam, int mapWidth, int mapHeight);
};

