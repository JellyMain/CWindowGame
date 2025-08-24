#include "Headers/draw.h"
#include "Headers/structs.h"
#include <SDL_image.h>

#include "Headers/app.h"
#include "Headers/window.h"
#include "Utils/ViewPortUtils.h"


List *InitDrawList()
{
	List *drawList = CreateList(0);
	return drawList;
}


void AddToDrawList(List *drawList, Entity *entity)
{
	AddToList(drawList, entity);
}


SDL_Texture *LoadTexture(char *fileName, SDL_Renderer *renderer)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading texture: %s", fileName);

	SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);

	if (!texture)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		               "Failed to load texture: %s, Error: %s", fileName, IMG_GetError());
	}

	return texture;
}


void Blit(SDL_Renderer *renderer, SDL_Texture *texture, Vector2 position, Vector2 scale)
{
	SDL_Rect rect;

	rect.x = position.x;
	rect.y = position.y;

	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.w *= scale.x;
	rect.h *= scale.y;

	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void PrepareScene(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);
}


void PresentScene(SDL_Renderer *renderer)
{
	SDL_RenderPresent(renderer);
}


void Render(App *app)
{
	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];
		SDL_Renderer *renderer = window->renderer;

		PrepareScene(renderer);

		for (int j = 0; j < app->drawList->size; j++)
		{
			Entity *entity = app->drawList->elements[j];

			Vector2 screenPos;

			switch (window->renderType)
			{
				case WINDOW_SCREEN_SPACE:
					screenPos = entity->worldPosition;
					break;

				case WINDOW_WORLD_SPACE:
					screenPos = WorldToScreen(entity->worldPosition, window->viewportOffset);
					break;

				default:
					screenPos = entity->worldPosition;
					printf("Unknown window type");
					break;
			}

			SDL_Texture *entityTexture = entity->texturesList->elements[i];
			Blit(renderer, entityTexture, screenPos, entity->scale);
		}

		PresentScene(renderer);
	}
}
