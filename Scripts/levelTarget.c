#include "Headers/levelTarget.h"

#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/structs.h"
#include "Headers/window.h"
#include "Utils/MathUtils.h"


Entity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale)
{
	Entity *levelTarget = malloc(sizeof(Entity));

	levelTarget->texturesList = CreateList(0);

	Vector2Int levelTargetSize = {0};

	for (int i = 0; i < app->drawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->drawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		SDL_Texture *texture = LoadTexture("D:/CWindowGame/Assets/LevelTarget.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &levelTargetSize.x, &levelTargetSize.y);
		AddToList(levelTarget->texturesList, texture);
	}

	levelTarget->renderType = SPRITE;
	levelTarget->worldPosition = position;
	levelTarget->scale = scale;
	levelTarget->size = levelTargetSize;

	AddToAllDrawLists(app, levelTarget);

	return levelTarget;
}


bool HasReachedLevelTarget(Entity *player, Entity *levelTarget)
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
