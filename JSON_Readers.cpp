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

std::vector<item> readItems(const std::string& filePath, SDL_Renderer* renderer)
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
        int atk = itemData["atk"];
        int def = itemData["def"];
        int price = itemData["price"];
        std::string imagelocation = itemData["imageLocation"];
        int itemType = itemData["itemType"];
        std::string description = itemData["description"];
        std::string name = itemData["name"];
        int lvl = itemData["lvl"];

        // Create the item using the parameterized constructor
        item newItem(atk, def, price, imagelocation, itemType, description, name, lvl, renderer);

        itemsList.push_back(newItem);
    }
    return itemsList;
}
