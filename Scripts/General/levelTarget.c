#include "Headers/levelTarget.h"

#include "../Infrastructure/Headers/update.h"
#include "../Render/Headers/draw.h"
#include "../Infrastructure/Headers/stateMachine.h"
#include "../General/Headers/structs.h"
#include "../Utils/Headers/mathUtils.h"
#include "../Render/Headers/textures.h"


void UpdateLevelTarget(void *data, App *app, float deltaTime);


GameEntity *CreateLevelTarget(App *app, Vector2Float position, Vector2Float scale)
{
	GameEntity *levelTarget = calloc(1, sizeof(GameEntity));


	Texture *texture = LoadTexture("LevelTarget.png");
	levelTarget->texture = texture;
	levelTarget->material = CreateMaterial(NULL, NULL);
	levelTarget->originalSize.x = levelTarget->texture->width;
	levelTarget->originalSize.y = levelTarget->texture->height;

	levelTarget->worldPosition = position;
	levelTarget->scale = scale;

	ListAdd(app->allGameEntities, levelTarget);

	Updatable *levelTargetUpdatable = CreateUpdatable(levelTarget, UpdateLevelTarget);
	AddUpdatable(app, levelTargetUpdatable);

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
