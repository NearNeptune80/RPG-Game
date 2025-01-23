#include "map.h"

map::map()
	: mapWidth(0), mapHeight(0)
{
}

map::~map()
{
}

void map::addLayer(const layer& newLayer)
{
    layers.push_back(newLayer);
    // Update map dimensions based on the new layer
    if (newLayer.layerWidth * TILE_SIZE > mapWidth)
        mapWidth = newLayer.layerWidth * TILE_SIZE;
    if (newLayer.layerHeight * TILE_SIZE > mapHeight)
        mapHeight = newLayer.layerHeight * TILE_SIZE;
}

void map::render(SDL_Renderer* renderer, const camera& camera) {
    for (const auto& lyr : layers) {
        for (const auto& tile : lyr.tiles) {
            // Calculate pixel position from tile position relative to camera
            int pixelX = tile.tileData.pos.x * TILE_SIZE - camera.x;
            int pixelY = tile.tileData.pos.y * TILE_SIZE - camera.y;

            // Only render tiles within the camera view
            if (pixelX + TILE_SIZE < 0 || pixelX > camera.width ||
                pixelY + TILE_SIZE < 0 || pixelY > camera.height) {
                continue;
            }

            SDL_Rect destRect = { pixelX, pixelY, TILE_SIZE, TILE_SIZE };

            // Render each texture in order
            for (const auto& texture : tile.tileTextures) {
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
            }
        }
    }
}