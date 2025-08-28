#include "Headers/text.h"
#include <SDL_ttf.h>

#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/structs.h"
#include "Headers/window.h"

Entity *CreateText(char *text, int fontSize, SDL_Color textColor, App *app, Window *window, Vector2Float scale)
{
	Entity *textEntity = malloc(sizeof(Entity));

	textEntity->texturesList = CreateList(0);
	textEntity->scale = scale;

	TTF_Font *font = TTF_OpenFont("D:/CWindowGame/Assets/Merchant Copy.ttf", fontSize);
	if (!font)
	{
		SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
		return false;
	}

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);

	if (!textSurface)
	{
		SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
		return false;
	}


	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(window->renderer, textSurface);

	AddToList(textEntity->texturesList, textTexture);

	if (!textTexture)
	{
		SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
		SDL_FreeSurface(textSurface);
		return false;
	}

	int textWidth = textSurface->w;
	int textHeight = textSurface->h;

	textEntity->worldPosition = (Vector2Int){
		(window->windowSize.x - textWidth) / 2,
		(window->windowSize.y - textHeight) / 2
	};

	textEntity->size = (Vector2Int){textWidth, textHeight};

	SDL_FreeSurface(textSurface);

	AddToDrawList(app->drawList, textEntity);

	return textEntity;
}
