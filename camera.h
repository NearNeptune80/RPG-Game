#pragma once

#include <SDL.h>

class camera
{
    public:
    int x; // Top-left x-coordinate
    int y; // Top-left y-coordinate
    int width;  // Width of the camera view (typically SCREEN_WIDTH)
    int height; // Height of the camera view (typically SCREEN_HEIGHT)

    camera(const int screenWidth, const int screenHeight);
    void update(int targetX, int targetY, int mapWidth, int mapHeight);
};

