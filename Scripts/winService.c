#include "Headers/winService.h"
#include <SDL_ttf.h>
#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/window.h"


bool CreateWinText(App *app, int fontSize, Window *winWindow)
{
	Entity *winText = malloc(sizeof(Entity));

	winText->texturesList = CreateList(0);
	winText->scale = (Vector2){1, 1};

	TTF_Font *font = TTF_OpenFont("D:/CWindowGame/Assets/Merchant Copy.ttf", fontSize);
	if (!font)
	{
		SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
		return false;
	}

	SDL_Color textColor = {255, 255, 255};

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "You Win!", textColor);

	if (!textSurface)
	{
		SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
		return false;
	}

	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];

		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(window->renderer, textSurface);

		AddToList(winText->texturesList, textTexture);

		if (!textTexture)
		{
			SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
			SDL_FreeSurface(textSurface);
			return false;
		}
	}

	int textWidth = textSurface->w;
	int textHeight = textSurface->h;

	winText->worldPosition = (Vector2){
		(winWindow->windowSize.x - textWidth) / 2,
		(winWindow->windowSize.y - textHeight) / 2
	};


	winText->size = (Vector2){textWidth, textHeight};

	SDL_FreeSurface(textSurface);

	AddToDrawList(app->drawList, winText);

	return true;
}


void CreateWinScreen(App *app, int fontSize)
{
	Window *winWindow = CreateGameWindowWithRenderer((Vector2){600, 600}, (Vector2){600, 600}, WINDOW_SCREEN_SPACE,
	                                                 "WinWindow");
	AddToList(app->windowsList, winWindow);


	if (!CreateWinText(app, fontSize, winWindow))
	{
		SDL_Log("Failed to create win text");
	}
}
