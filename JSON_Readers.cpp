#include "JSON_Readers.h"
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <vector>
#include <string>

using json = nlohmann::json;

std::vector<mapData> readMapData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map data file: " << filePath << std::endl;
        return {};
    }

    json mapDataJson;
    file >> mapDataJson;

    std::vector<mapData> mapDataList;
    for (const auto& layerData : mapDataJson["layers"]) {
        for (const auto& row : layerData["layout"]) {
            for (const auto& tileData : row) {
                mapData tile;
                tile.tileType = tileData["tileType"];
                if (tileData["solid"] == 1) {
                    tile.solid = true;
                }
                else {
                    tile.solid = false;
                }
                tile.trigger = tileData["trigger"];
                if (tileData.contains("triggerData")) {
                    tile.triggerData = tileData["triggerData"];
                }
                mapDataList.push_back(tile);
            }
        }
    }
    return mapDataList;
}

std::vector<mapInfo> readMapIndex(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map index file: " << filePath << std::endl;
        return {};
    }

    json mapIndexJson;
    file >> mapIndexJson;

    std::vector<mapInfo> mapIndexList;
    for (const auto& mapData : mapIndexJson["maps"]) {
        mapInfo map;
        map.mapId = mapData["id"];
        map.mapName = mapData["name"];
        map.filePath = mapData["file"];
        mapIndexList.push_back(map);
    }
    return mapIndexList;
}

std::vector<item> readItems(const std::string& filePath)
{
	std::ifstream file(filePath);
    if (!file.is_open())
    {
		std::cerr << "Failed to open items file: " << filePath << std::endl;
		return {};
    }

	json itemsJson;
	file >> itemsJson;

	std::vector<item> itemsList;
    for (const auto& itemData : itemsJson["items"])
    {
		item newItem;
		newItem.atk = itemData["atk"];
		newItem.def = itemData["def"];
		newItem.price = itemData["price"];
		newItem.imagelocation = itemData["imageLocation"];
		newItem.itemType = itemData["itemType"];
		newItem.description = itemData["description"];
		newItem.name = itemData["name"];
		newItem.lvl = itemData["lvl"];
		itemsList.push_back(newItem);
    }
    return itemsList;
}
