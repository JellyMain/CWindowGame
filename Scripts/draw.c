#include "Headers/draw.h"
#include "Headers/structs.h"
#include <SDL_image.h>
#include "Headers/ui.h"
#include "DataStructures/Headers/Dictionary.h"
#include "Headers/app.h"
#include "Headers/window.h"
#include "Utils/viewPortUtils.h"


Dictionary *InitDrawDictionary()
{
	//Window - List of entities dictionary
	Dictionary *drawDictionary = CreateDictionary(HashWindow, WindowEquals);
	return drawDictionary;
}


void AddToAllGameEntitiesDrawLists(App *app, GameEntity *entity)
{
	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		List *drawList = pair->value;

		AddToList(drawList, entity);
	}

	AddToList(app->allGameEntities, entity);
}


void AddToGameEntitiesDrawList(App *app, Window *window, GameEntity *entity)
{
	List *drawList = GetFromDictionary(app->gameEntitiesDrawDictionary, window);
	AddToList(drawList, entity);
	AddToList(app->allGameEntities, entity);
}


void AddToUIEntitiesDrawList(App *app, Window *window, UIEntity *entity)
{
	List *drawList = GetFromDictionary(app->uiEntitiesDrawDictionary, window);
	AddToList(drawList, entity);
	AddToList(app->allUIEntities, entity);
}


void AddToGizmoEntitiesDrawList(App *app, Window *window, GizmoEntity *gizmoEntity)
{
	List *drawList = GetFromDictionary(app->gizmosEntitiesDrawDictionary, window);
	AddToList(drawList, gizmoEntity);
	AddToList(app->allGizmosEntities, gizmoEntity);
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


void BlitGameEntity(SDL_Renderer *renderer, SDL_Texture *texture, Vector2Int position, GameEntity *entity)
{
	SDL_Rect rect;

	entity->size.x = entity->originalSize.x * entity->scale.x;
	entity->size.y = entity->originalSize.y * entity->scale.y;

	rect.x = position.x - entity->size.x / 2;
	rect.y = position.y - entity->size.y / 2;
	rect.w = entity->size.x;
	rect.h = entity->size.y;

	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void BlitUIEntity(SDL_Renderer *renderer, SDL_Texture *texture, Vector2Int position, UIEntity *entity)
{
	SDL_Rect rect;

	entity->size.x = entity->originalSize.x * entity->scale.x;
	entity->size.y = entity->originalSize.y * entity->scale.y;

	rect.x = position.x - entity->size.x / 2;
	rect.y = position.y - entity->size.y / 2;
	rect.w = entity->size.x;
	rect.h = entity->size.y;

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
	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *gameEntitiesPair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = gameEntitiesPair->key;
		List *gameEntitiesDrawList = gameEntitiesPair->value;
		List *uiEntitiesDrawList = GetFromDictionary(app->uiEntitiesDrawDictionary, window);
		List *gizmoEntitiesDrawList = GetFromDictionary(app->gizmosEntitiesDrawDictionary, window);
		SDL_Renderer *renderer = window->renderer;

		PrepareScene(renderer);

		for (int j = 0; j < gameEntitiesDrawList->size; j++)
		{
			GameEntity *entity = gameEntitiesDrawList->elements[j];

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

			SDL_Texture *entityTexture = entity->texturesList->elements[i];

			BlitGameEntity(renderer, entityTexture, screenPos, entity);
		}


		for (int j = 0; j < uiEntitiesDrawList->size; j++)
		{
			UIEntity *entity = uiEntitiesDrawList->elements[j];
			SDL_Texture *entityTexture = entity->texture;
			Vector2Int screenPos = entity->worldPosition;
			BlitUIEntity(renderer, entityTexture, screenPos, entity);
		}


		if (app->showGizmos)
		{
			SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);


			for (int k = 0; k < gizmoEntitiesDrawList->size; k++)
			{
				GizmoEntity *gizmoEntity = gizmoEntitiesDrawList->elements[k];

				SDL_SetRenderDrawColor(window->renderer,
				                       gizmoEntity->color.r,
				                       gizmoEntity->color.g,
				                       gizmoEntity->color.b,
				                       gizmoEntity->color.a);


				DrawThickRectBorder(window, gizmoEntity->connectedEntity->worldPosition,
				                    gizmoEntity->connectedEntity->size, gizmoEntity->thickness);


				char entityInfo[150];
				snprintf(entityInfo, sizeof(entityInfo), "x:%d,y:%d w:%d,h%d",
				         gizmoEntity->connectedEntity->worldPosition.x,
				         gizmoEntity->connectedEntity->worldPosition.y,
				         gizmoEntity->connectedEntity->size.x,
				         gizmoEntity->connectedEntity->size.y);


				DrawDynamicText(window, app->textAtlas, entityInfo, (Vector2Int){
					                gizmoEntity->connectedEntity->worldPosition.x - gizmoEntity->connectedEntity->size.x
					                / 2,
					                gizmoEntity->connectedEntity->worldPosition.y - gizmoEntity->connectedEntity->size.y
					                / 2 - 10
				                }, (Vector2Float){1, 1});
			}
		}

		PresentScene(renderer);
	}
}
