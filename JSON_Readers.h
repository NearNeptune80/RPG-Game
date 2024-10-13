#pragma once

#include <vector>
#include <string>
#include "item.h"

struct mapInfo {
	int mapId;
	std::string mapName;
	std::string filePath;
};

struct mapData {
	int tileType;
	bool solid;
	int trigger;
	std::string triggerData;
};

std::vector<mapData> readMapData(const std::string& filePath);
std::vector<mapInfo>readMapIndex(const std::string& filePath);
std::vector<item> readItems(const std::string& filePath);