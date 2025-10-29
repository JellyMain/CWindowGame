#include "Headers/levelTarget.h"

#include "Headers/update.h"
#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/stateMachine.h"
#include "Headers/structs.h"
#include "Headers/window.h"
#include "Utils/mathUtils.h"


GameEntity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale)
{
	GameEntity *levelTarget = calloc(1, sizeof(GameEntity));

	levelTarget->texturesList = ListCreate(0);

	Vector2Int levelTargetSize = {0};

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		SDL_Texture *texture = LoadTexture("LevelTarget.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &levelTargetSize.x, &levelTargetSize.y);
		ListAdd(levelTarget->texturesList, texture);
	}

	levelTarget->worldPosition = position;
	levelTarget->scale = scale;
	levelTarget->originalSize = levelTargetSize;

	AddToAllGameEntitiesDrawLists(app, levelTarget);

	return levelTarget;
}


bool HasReachedLevelTarget(GameEntity *player, GameEntity *levelTarget)
{
	return IsEntityOverlapping(player, player->worldPosition, levelTarget, levelTarget->worldPosition);
}


void UpdateLevelTarget(void *data, App *app, float deltaTime)
{
	if (app->hasWon == false)
	{
		if (HasReachedLevelTarget(app->levelData->player, app->levelData->levelTarget))
		{
			app->hasWon = true;
			SetPendingState(app, GAME_OVER_GAME_STATE);
		}
	}
}


Updatable *CreateLevelTargetUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateLevelTarget);
	return updatable;
}
