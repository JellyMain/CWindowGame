#include "../Services/Headers/levelService.h"

#include "../General/Headers/levelTarget.h"
#include "../General/Headers/player.h"
#include "../General/Headers/structs.h"
#include "../Infrastructure/Headers/update.h"
#include "../Infrastructure/Headers/window.h"


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


	app->levelData->levelTarget = CreateLevelTarget(app, (Vector2Int){500, 500}, (Vector2Float){1, 1});
	app->levelData->player = CreatePlayer(app, (Vector2Int){300, 300}, (Vector2Float){1, 1});
}


LevelData *CreateLevelData()
{
	LevelData *levelData = calloc(1, sizeof(LevelData));
	levelData->player = NULL;
	levelData->levelTarget = NULL;
	return levelData;
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

	if (entity->material->texture != NULL)
	{
		glDeleteTextures(1, &entity->material->texture->textureId);
		glDeleteProgram(entity->material->shaderProgram);
		free(entity->material->texture);
	}

	free(entity);
}


void CleanUpUIEntity(UIEntity *entity)
{
	if (entity == NULL)
	{
		return;
	}

	ListDestroy(entity->childEntities);

	if (entity->material->texture != NULL)
	{
		glDeleteTextures(1, &entity->material->texture->textureId);
		glDeleteProgram(entity->material->shaderProgram);
		free(entity->material->texture);
	}

	free(entity);
}


void CleanUpScene(App *app)
{
	for (int i = 0; i < app->updateSystem->updatables->size; ++i)
	{
		Updatable *updatable = app->updateSystem->updatables->elements[i];
		DestroyUpdatable(updatable);
	}


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

	ListClear(app->updateSystem->updatables);
	ListClear(app->allUIEntities);
	ListClear(app->allGameEntities);
	ListClear(app->allGizmosEntities);

	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];

		// SDL_Texture *atlasTexture = DictionaryGet(app->textAtlas->windowTexturesDictionary, window);
		// SDL_DestroyTexture(atlasTexture);
		CleanUpWindow(window);
	}

	ListClear(app->allWindows);
	// DictionaryClear(app->textAtlas->windowTexturesDictionary);
	DictionaryClear(app->tweenTargetsDictionary);
}
