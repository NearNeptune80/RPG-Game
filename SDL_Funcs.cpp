#include "SDL_Funcs.h"
#include <iostream>
#include <string>
#include <SDL.h>

bool init(SDL_Window* &window, SDL_Surface* &screenSurface, SDL_Renderer* &renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}
	else {
		window = SDL_CreateWindow("This is a new window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				return false;
			}
			else {
				screenSurface = SDL_GetWindowSurface(window);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
				SDL_UpdateWindowSurface(window);
				if (TTF_Init() == -1) {
					std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
					return -1;
				}
				return true;
			}
		}
	}
}

bool quit(SDL_Window* &window, SDL_Renderer* &renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return true;
}