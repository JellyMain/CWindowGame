#include <SDL_events.h>
#include <SDL_timer.h>

#include "Headers/ui.h"
#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/draw.h"
#include "Headers/initSDL2.h"
#include "Headers/structs.h"
#include "Headers/app.h"
#include "Headers/input.h"
#include "Headers/winService.h"


static App *g_app = NULL;

int EventFilter(void *userdata, SDL_Event *event)
{
	if (event->type == SDL_SYSWMEVENT || event->type == SDL_WINDOWEVENT)
	{
		if (g_app)
		{
			Render(g_app);

			for (int i = 0; i < g_app->gameEntitiesDrawDictionary->allPairs->size; i++)
			{
				KeyValuePair *pair = g_app->gameEntitiesDrawDictionary->allPairs->elements[i];
				Window *window = pair->key;
				UpdateWindow(g_app, window);
			}
		}
	}

	return 1;
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

	DestroyList(entity->texturesList);
	free(entity);
}


void CleanUpUIEntity(UIEntity *entity)
{
	if (entity == NULL)
	{
		return;
	}

	DestroyList(entity->childEntities);
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

	ClearList(app->allUIEntities);
	ClearList(app->allGameEntities);
	ClearList(app->allGizmosEntities);

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		List *gameEntitiesDrawList = pair->value;
		List *uiEntitiesDrawList = GetFromDictionary(app->uiEntitiesDrawDictionary, window);
		List *gizmoEntitiesDrawList = GetFromDictionary(app->gizmosEntitiesDrawDictionary, window);

		SDL_Texture *atlasTexture = GetFromDictionary(app->textAtlas->windowTexturesDictionary, window);
		SDL_DestroyTexture(atlasTexture);

		ClearList(gizmoEntitiesDrawList);
		ClearList(uiEntitiesDrawList);
		ClearList(gameEntitiesDrawList);
		CleanUpWindow(window);
	}

	ClearDictionary(app->textAtlas->windowTexturesDictionary);
	ClearDictionary(app->gizmosEntitiesDrawDictionary);
	ClearDictionary(app->gameEntitiesDrawDictionary);
	ClearDictionary(app->uiEntitiesDrawDictionary);
}


int main()
{
	App *app = CreateApp(true);

	if (InitSDL2(app) != 0)
	{
		return 1;
	}



	GameEntity *player = CreatePlayer(app, (Vector2Int){100, 100}, (Vector2Float){2, 2});
	GameEntity *levelTarget = CreateLevelTarget(app, (Vector2Int){500, 500}, (Vector2Float){2, 2});

	g_app = app;

	SDL_SetEventFilter(EventFilter, NULL);

	while (1)
	{
		ProcessInput();

		Render(app);


		for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
		{
			KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
			Window *window = pair->key;
			UpdateWindow(app, window);
		}

		UpdateUIElements(app);

		ProcessInput();

		MovePlayer(app, player);

		if (HasReachedLevelTarget(player, levelTarget))
		{
			if (app->hasWon == false)
			{
				SDL_SetEventFilter(NULL, NULL);
				CleanUpLevel(app);
				CreateWinScreen(app, 64);
				app->hasWon = true;
				SDL_SetEventFilter(EventFilter, NULL);
			}
		}

		SDL_Delay(16);
	}

	return 0;
}
