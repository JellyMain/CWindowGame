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

bool IsPositionInBounds(App *app, GameEntity *entity, Vector2Float position)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];

		Vector2Float boundsMin = {
			window->position.x, window->position.y
		};
		Vector2Float boundsMax = {
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


GameEntity *CreatePlayer(App *app, Vector2Float position, Vector2Float scale)
{
	GameEntity *player = calloc(1, sizeof(GameEntity));

	Texture *texture = LoadTexture("SlimeEnemy.png");

	player->texture = texture;
	player->material = CreateMaterial(NULL, NULL);

	player->worldPosition = position;
	player->scale = scale;
	player->originalSize.x = player->texture->width;
	player->originalSize.y = player->texture->height;

	ListAdd(app->allGameEntities, player);

	Updatable *playerUpdatable = CreateUpdatable(player, UpdatePlayer);
	AddUpdatable(app, playerUpdatable);

	return player;
}


void MovePlayer(App *app, GameEntity *player)
{
	Vector2Float direction = GetMoveDirection();

	if (direction.x == 0 && direction.y == 0)
	{
		return;
	}

	Vector2Float nextFramePosition = {
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
