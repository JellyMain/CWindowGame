#include "Headers/levelService.h"

#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/structs.h"
#include "Headers/window.h"


void CreateLevel(App *app, int level)
{
	CreateGameWindowWithRenderer(app, (Vector2Int){300, 100}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             SCALE_WITH_RESIZE,
	                             "WindowA");
	CreateGameWindowWithRenderer(app, (Vector2Int){100, 100}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             FIXED_SIZE,
	                             "WindowB");
	CreateGameWindowWithRenderer(app, (Vector2Int){500, 300}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             FIXED_SIZE,
	                             "WindowC");


	app->player = CreatePlayer(app, (Vector2Int){300, 300}, (Vector2Float){2, 2});
	app->levelTarget = CreateLevelTarget(app, (Vector2Int){500, 500}, (Vector2Float){2, 2});
}



void CleanUpWindow(Window *window)
{
	if (window == NULL)
	{
		return;
	}

	SDL_DestroyWindow(window->sdlWindow);
	SDL_DestroyRenderer(window->renderer);

	free(window);
}


void CleanUpGameEntity(GameEntity *entity)
{
	if (entity == NULL)
	{
		return;
	}

	for (int i = 0; i < entity->texturesList->size; ++i)
	{
		SDL_DestroyTexture(entity->texturesList->elements[i]);
	}

	ListDestroy(entity->texturesList);
	free(entity);
}


void CleanUpUIEntity(UIEntity *entity)
{
	if (entity == NULL)
	{
		return;
	}

	ListDestroy(entity->childEntities);
	SDL_DestroyTexture(entity->texture);
	free(entity);
}


void CleanUpLevel(App *app)
{
	for (int i = 0; i < app->allGizmosEntities->size; i++)
	{
		GizmoEntity *gizmoEntity = app->allGizmosEntities->elements[i];
		free(gizmoEntity);
	}

	for (int i = 0; i < app->allGameEntities->size; i++)
	{
		GameEntity *entity = app->allGameEntities->elements[i];
		CleanUpGameEntity(entity);
	}

	for (int i = 0; i < app->allUIEntities->size; i++)
	{
		UIEntity *uiEntity = app->allUIEntities->elements[i];
		CleanUpUIEntity(uiEntity);
	}

	ListClear(app->allUIEntities);
	ListClear(app->allGameEntities);
	ListClear(app->allGizmosEntities);

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		List *gameEntitiesDrawList = pair->value;
		List *uiEntitiesDrawList = DictionaryGet(app->uiEntitiesDrawDictionary, window);
		List *gizmoEntitiesDrawList = DictionaryGet(app->gizmosEntitiesDrawDictionary, window);

		SDL_Texture *atlasTexture = DictionaryGet(app->textAtlas->windowTexturesDictionary, window);
		SDL_DestroyTexture(atlasTexture);

		ListClear(gizmoEntitiesDrawList);
		ListClear(uiEntitiesDrawList);
		ListClear(gameEntitiesDrawList);
		CleanUpWindow(window);
	}

	DictionaryClear(app->textAtlas->windowTexturesDictionary);
	DictionaryClear(app->gizmosEntitiesDrawDictionary);
	DictionaryClear(app->gameEntitiesDrawDictionary);
	DictionaryClear(app->uiEntitiesDrawDictionary);
	DictionaryClear(app->tweenTargetsDictionary);
}
