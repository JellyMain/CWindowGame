#include "Headers/levelTarget.h"

#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/structs.h"
#include "Headers/window.h"


Entity *CreateLevelTarget(App *app, Vector2Int position, Vector2Int scale)
{
	Entity *levelTarget = malloc(sizeof(Entity));

	levelTarget->texturesList = CreateList(0);

	Vector2Int levelTargetSize = {0};

	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];
		SDL_Texture *texture = LoadTexture("D:/CWindowGame/Assets/LevelTarget.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &levelTargetSize.x, &levelTargetSize.y);
		AddToList(levelTarget->texturesList, texture);
	}


	levelTargetSize.x *= scale.x;
	levelTargetSize.y *= scale.y;

	levelTarget->worldPosition = position;
	levelTarget->scale = scale;
	levelTarget->size = levelTargetSize;

	AddToDrawList(app->drawList, levelTarget);

	return levelTarget;
}


bool HasReachedLevelTarget(Entity *player, Entity *levelTarget)
{
	int playerLeft = player->worldPosition.x;
	int playerRight = player->worldPosition.x + player->size.x;
	int playerTop = player->worldPosition.y;
	int playerBottom = player->worldPosition.y + player->size.y;

	int targetLeft = levelTarget->worldPosition.x;
	int targetRight = levelTarget->worldPosition.x + levelTarget->size.x;
	int targetTop = levelTarget->worldPosition.y;
	int targetBottom = levelTarget->worldPosition.y + levelTarget->size.y;

	if (playerRight > targetLeft &&
	    playerLeft < targetRight &&
	    playerBottom > targetTop &&
	    playerTop < targetBottom)
	{
		return true;
	}

	return false;
}
