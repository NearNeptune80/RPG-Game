#include "JSON_Readers.h"
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <vector>
#include <string>

using json = nlohmann::json;

map createMap(std::string mapLocation, std::vector<SDL_Texture*> tileTextures)
{
    std::ifstream file(mapLocation);
    if (!file.is_open())
    {
        std::cerr << "Failed to open map file: " << mapLocation << std::endl;
        return map();
    }

    json mapJson;
    file >> mapJson;

    map newMap;

    for (const auto& layerData : mapJson["layers"])
    {
        const auto& layout = layerData["layout"];
        int height = layout.size();
        int width = height > 0 ? layout[0].size() : 0;
        layer newLayer(width, height);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const auto& tileData = layout[y][x];
                if (tileData["textures"].empty())
                    continue; // Skip empty tiles

                // Extract textures
                std::vector<SDL_Texture*> textures;
                for (const auto& texIndex : tileData["textures"])
                {
                    int index = texIndex.get<int>();
                    if (index >= 0 && index < static_cast<int>(tileTextures.size()))
                    {
                        textures.push_back(tileTextures[index]);
                    }
                    else
                    {
                        std::cerr << "Texture index out of bounds: " << index << std::endl;
                    }
                }

                // Extract solid
                bool solid = tileData["solid"].get<bool>();

                // Determine tileType and extract trigger data if any
                int tileType = 0; // Default: standard tile
                std::optional<std::string> actionData;
                std::optional<int> destX;
                std::optional<int> destY;

                if (tileData.contains("trigger"))
                {
                    std::string triggerType = tileData["trigger"].get<std::string>();
                    if (triggerType == "shop")
                    {
                        tileType = 2;
                        if (tileData.contains("triggerData"))
                        {
                            actionData = tileData["triggerData"].get<std::string>();
                        }
                    }
                    else if (triggerType == "transport")
                    {
                        tileType = 1;
                        if (tileData.contains("triggerData"))
                        {
                            const auto& triggerData = tileData["triggerData"];
                            if (triggerData.contains("destinationMap"))
                            {
                                actionData = triggerData["destinationMap"].get<std::string>();
                            }
                            if (triggerData.contains("destinationCoordinates"))
                            {
                                destX = triggerData["destinationCoordinates"]["x"].get<int>();
                                destY = triggerData["destinationCoordinates"]["y"].get<int>();
                            }
                        }
                    }
                }

                // Create and add the tile
                tile newTile(x, y, solid, tileType, textures, actionData, destX, destY);
                newLayer.addTile(newTile);
            }
        }

        newMap.addLayer(newLayer);
    }

    return newMap;
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

std::vector<SDL_Texture*> loadTileTextures(SDL_Renderer* renderer, SDL_Surface* tilesetSurface, int columns, int rows, int tileWidth, int tileHeight)
{
    std::vector<SDL_Texture*> tileTextures;

    // Create a texture from the tileset surface
    SDL_Texture* tilesetTexture = SDL_CreateTextureFromSurface(renderer, tilesetSurface);
    if (!tilesetTexture)
    {
        std::cerr << "Failed to create tileset texture: " << SDL_GetError() << std::endl;
        return tileTextures;
    }

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            SDL_Rect srcRect = { col * tileWidth, row * tileHeight, tileWidth, tileHeight };
            SDL_Surface* tileSurface = SDL_CreateRGBSurface(0, tileWidth, tileHeight, tilesetSurface->format->BitsPerPixel,
                tilesetSurface->format->Rmask,
                tilesetSurface->format->Gmask,
                tilesetSurface->format->Bmask,
                tilesetSurface->format->Amask);
            if (!tileSurface)
            {
                std::cerr << "Failed to create tile surface: " << SDL_GetError() << std::endl;
                continue;
            }

            // Blit the specific tile from the tileset to the tile surface
            if (SDL_BlitSurface(tilesetSurface, &srcRect, tileSurface, NULL) != 0)
            {
                std::cerr << "Failed to blit tile surface: " << SDL_GetError() << std::endl;
                SDL_FreeSurface(tileSurface);
                continue;
            }

            // Create a texture from the tile surface
            SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
            if (!tileTexture)
            {
                std::cerr << "Failed to create tile texture: " << SDL_GetError() << std::endl;
                SDL_FreeSurface(tileSurface);
                continue;
            }

            tileTextures.push_back(tileTexture);
            SDL_FreeSurface(tileSurface);
        }
    }

    SDL_DestroyTexture(tilesetTexture); // Cleanup the intermediate tileset texture
    return tileTextures;
}