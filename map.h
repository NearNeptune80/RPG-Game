#pragma once

#include <SDL.h>
#include <vector>
#include "layer.h"
#include "camera.h"

class map
{
public:
    map();
    ~map();

    void addLayer(const layer& newLayer);
    void render(SDL_Renderer* renderer, const camera& camera);

	int mapWidth;
    int mapHeight;

    std::vector<layer> layers;
    const int TILE_SIZE = 32;
};