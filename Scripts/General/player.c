#include "Headers/player.h"

#include "../Infrastructure/Headers/stateMachine.h"
#include "../Infrastructure/Headers/update.h"
#include "../Render/Headers/draw.h"
#include "../Input/Headers/input.h"
#include "Headers/structs.h"
#include "../Utils/Headers/mathUtils.h"
#include "../Render/Headers/textures.h"

#define MOVE_SPEED 5

void UpdatePlayer(void *data, App *app, float deltaTime);

bool IsPositionInBounds(App *app, GameEntity *entity, Vector2Int position)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];

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

	Vector2Int playerSize = {0};

	Texture *texture = LoadTexture("SlimeEnemy.png");

	player->material = CreateMaterial(NULL, NULL, texture);

	playerSize.x = player->material->texture->width;
	playerSize.y = player->material->texture->height;

	player->worldPosition = position;
	player->scale = scale;
	player->originalSize = playerSize;

	ListAdd(app->allGameEntities, player);

	Updatable *playerUpdatable = CreateUpdatable(player, UpdatePlayer);
	AddUpdatable(app, playerUpdatable);

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
