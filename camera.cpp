#include "camera.h"

camera::camera(const int screenWidth, const int screenHeight)
	: x(0), y(0), width(screenWidth), height(screenHeight)
{
}

void camera::update(int targetX, int targetY, int mapWidth, int mapHeight)
{
    // Center the camera on the target(
    if (targetX + width < mapWidth && targetY + height < mapHeight)
    {
		x = targetX - width / 2;
		y = targetY - height / 2;
    }
	else if (targetX + width > mapWidth || targetX < 0)
	{
		y = targetY - height / 2;
	}
	else if (targetY + height > mapHeight || targetY < 0)
	{
		x = targetX - width / 2;
	}

    // Clamp the camera to the map boundaries
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > mapWidth) x = mapWidth - width;
    if (y + height > mapHeight) y = mapHeight - height;
}
