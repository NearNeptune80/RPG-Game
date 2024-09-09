#pragma once

#include <vector>
#include <string>

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
//readItems();
