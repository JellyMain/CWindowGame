#include "Headers/levelTarget.h"

#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/structs.h"
#include "Headers/window.h"
#include "Utils/MathUtils.h"


GameEntity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale)
{
	GameEntity *levelTarget = malloc(sizeof(GameEntity));

	levelTarget->texturesList = CreateList(0);

	Vector2Int levelTargetSize = {0};

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		SDL_Texture *texture = LoadTexture("D:/CWindowGame/Assets/LevelTarget.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &levelTargetSize.x, &levelTargetSize.y);
		AddToList(levelTarget->texturesList, texture);
	}

	levelTarget->worldPosition = position;
	levelTarget->scale = scale;
	levelTarget->size = levelTargetSize;

	AddToAllGameEntitiesDrawLists(app, levelTarget);

	return levelTarget;
}


bool HasReachedLevelTarget(GameEntity *player, GameEntity *levelTarget)
{
	Vector2Int boundsMin = {
		levelTarget->worldPosition.x, levelTarget->worldPosition.y
	};
	Vector2Int boundsMax = {
		levelTarget->worldPosition.x + levelTarget->size.x * levelTarget->scale.x,
		levelTarget->worldPosition.y + levelTarget->size.y * levelTarget->scale.y
	};

	if (IsEntityInBounds(player, player->worldPosition, boundsMin, boundsMax))
	{
		return true;
	}

	return false;
}
