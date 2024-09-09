#pragma once

#include <SDL.h>

bool init(SDL_Window*& window, SDL_Surface*& screenSurface, SDL_Renderer* &renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
bool quit(SDL_Window*& window, SDL_Renderer* &renderer);