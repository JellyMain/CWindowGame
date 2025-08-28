#include "Headers/draw.h"
#include "Headers/structs.h"
#include <SDL_image.h>

#include "DataStructures/Headers/Dictionary.h"
#include "Headers/app.h"
#include "Headers/window.h"
#include "Utils/ViewPortUtils.h"


Dictionary *InitDrawDictionary()
{
	//Window - List of entities dictionary
	Dictionary *drawDictionary = CreateDictionary(HashWindow, WindowEquals);
	return drawDictionary;
}


void AddToAllDrawLists(App *app, Entity *entity)
{
	for (int i = 0; i < app->drawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->drawDictionary->allPairs->elements[i];
		List *drawList = pair->value;

		AddToList(drawList, entity);
	}

	AddToList(app->allEntities, entity);
}


void AddToWindowDrawList(App *app, Window *window, Entity *entity)
{
	List *drawList = GetFromDictionary(app->drawDictionary, window);
	AddToList(drawList, entity);
	AddToList(app->allEntities, entity);
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


void Blit(SDL_Renderer *renderer, SDL_Texture *texture, Vector2Int position, Entity *entity)
{
	SDL_Rect rect;

	rect.x = position.x;
	rect.y = position.y;

	rect.w = entity->size.x * entity->scale.x;
	rect.h = entity->size.y * entity->scale.y;

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
	for (int i = 0; i < app->drawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->drawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		List *drawList = pair->value;
		SDL_Renderer *renderer = window->renderer;

		PrepareScene(renderer);

		for (int j = 0; j < drawList->size; j++)
		{
			Entity *entity = drawList->elements[j];

			Vector2Int screenPos;

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

			SDL_Texture *entityTexture;

			if (entity->renderType == UI)
			{
				entityTexture = entity->texturesList->elements[0];
			}
			else
			{
				entityTexture = entity->texturesList->elements[i];
			}

			Blit(renderer, entityTexture, screenPos, entity);
		}

		PresentScene(renderer);
	}
}
