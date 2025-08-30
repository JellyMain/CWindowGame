#include "Headers/player.h"

#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/structs.h"
#include "Utils/MathUtils.h"
#define MOVE_SPEED 5


bool IsPositionInBounds(App *app, GameEntity *entity, Vector2Int position)
{
	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;

		Vector2Int boundsMin = {
			window->windowPosition.x, window->windowPosition.y
		};
		Vector2Int boundsMax = {
			window->windowPosition.x + window->windowSize.x,
			window->windowPosition.y + window->windowSize.y
		};

		if (IsEntityInBounds(entity, position, boundsMin, boundsMax))
		{
			return true;
		}
	}

	return false;
}


GameEntity *CreatePlayer(App *app, Vector2Int position, Vector2Float scale)
{
	GameEntity *player = malloc(sizeof(GameEntity));

	player->texturesList = CreateList(0);

	Vector2Int playerSize = {0};

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		SDL_Texture *texture = LoadTexture("D:/CWindowGame/Assets/SlimeEnemy.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &playerSize.x, &playerSize.y);
		AddToList(player->texturesList, texture);
	}

	player->worldPosition = position;
	player->scale = scale;
	player->size = playerSize;

	AddToAllGameEntitiesDrawLists(app, player);

	return player;
}


void MovePlayer(App *app, GameEntity *player)
{
	Vector2Int direction = GetMoveDirection();

	if (direction.x == 0 && direction.y == 0)
	{
		return;
	}

	Vector2Int nextFramePosition = {
		player->worldPosition.x + direction.x * MOVE_SPEED, player->worldPosition.y + direction.y * MOVE_SPEED
	};

	if (IsPositionInBounds(app, player, nextFramePosition))
	{
		player->worldPosition = nextFramePosition;
	}
}
