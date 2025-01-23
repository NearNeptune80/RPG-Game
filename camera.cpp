#include "camera.h"

camera::camera(const int screenWidth, const int screenHeight)
	: x(0), y(0), width(screenWidth), height(screenHeight)
{
}

void camera::update(int targetX, int targetY, int mapWidth, int mapHeight)
{
    // Center the camera on the target
    x = targetX - width / 2;
    y = targetY - height / 2;

    // Clamp the camera to the map boundaries
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > mapWidth - width) x = mapWidth - width;
    if (y > mapHeight - height) y = mapHeight - height;
}
