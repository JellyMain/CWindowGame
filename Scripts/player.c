#include "Headers/player.h"

#include "Headers/update.h"
#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/structs.h"
#include "Utils/mathUtils.h"
#define MOVE_SPEED 5


bool IsPositionInBounds(App *app, GameEntity *entity, Vector2Int position)
{
	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;

		Vector2Int boundsMin = {
			window->position.x, window->position.y
		};
		Vector2Int boundsMax = {
			window->position.x + window->size.x,
			window->position.y + window->size.y
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
	GameEntity *player = calloc(1, sizeof(GameEntity));

	player->texturesList = ListCreate(0);

	Vector2Int playerSize = {0};

	for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
	{
		KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
		Window *window = pair->key;
		SDL_Texture *texture = LoadTexture("SlimeEnemy.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &playerSize.x, &playerSize.y);
		ListAdd(player->texturesList, texture);
	}

	player->worldPosition = position;
	player->scale = scale;
	player->originalSize = playerSize;

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


void UpdatePlayer(void *data, App *app, float deltaTime)
{
	GameEntity *player = data;
	MovePlayer(app, player);
}


Updatable *CreatePlayerUpdatable(GameEntity *player)
{
	Updatable *updatable = CreateUpdatable(player, UpdatePlayer);
	return updatable;
}
