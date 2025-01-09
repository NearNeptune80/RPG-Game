#include "item.h"

item::item() : atk(0), def(0), price(0), itemType(-1), lvl(0), texture(nullptr)
{
}

item::item(int atk, int def, int price, const std::string& imagelocation,
	int itemType, const std::string& description, const std::string& name,
	int lvl, SDL_Renderer* renderer)
	: atk(atk), def(def), price(price), imagelocation(imagelocation),
	itemType(itemType), description(description), name(name), lvl(lvl)
{
	if (renderer && !imagelocation.empty())
	{
		SDL_Surface* surface = IMG_Load(imagelocation.c_str());
		if (surface)
		{
			SDL_Texture* rawTexture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			if (rawTexture)
			{
				texture = std::shared_ptr<SDL_Texture>(rawTexture, SDL_DestroyTexture);
			}
			else
			{
				std::cout << "Could not create texture: " << SDL_GetError() << std::endl;
			}
		}
		else
		{
			std::cout << "Could not load image '" << imagelocation << "': " << IMG_GetError() << std::endl;
		}
	}
}

item::~item()
{
	if (texture)
	{
		//SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

std::string item::getItemTypeName()
{
	switch (itemType)
	{
	case 0:
		return "Helmet";
		break;
	case 1:
		return "Chestplate";
		break;
	case 2:
		return "Leggings";
		break;
	case 3:
		return "Boots";
		break;
	case 4:
		return "Weapon";
		break;
	case 5:
		return "Shield";
		break;
	case 6:
		return "Accessory";
		break;
	case 7:
		return "Consumable";
		break;
	default:
		return "Error";
		break;
	}
}
