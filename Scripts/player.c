#include "Headers/player.h"

#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/structs.h"
#define MOVE_SPEED 5


bool IsPositionInBounds(App *app, Entity *entity, Vector2 position)
{
	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];

		int windowLeft = window->windowPosition.x - entity->size.x / 2;
		int windowTop = window->windowPosition.y - entity->size.y / 2;

		int windowRight = window->windowPosition.x + window->windowSize.x - entity->size.x / 2;
		int windowBottom = window->windowPosition.y + window->windowSize.y - entity->size.y / 2;

		if (position.x >= windowLeft &&
		    position.x <= windowRight &&
		    position.y >= windowTop &&
		    position.y <= windowBottom)
		{
			return true;
		}
	}

	return false;
}


Entity *CreatePlayer(App *app, Vector2 position, Vector2 scale)
{
	Entity *player = malloc(sizeof(Entity));

	player->texturesList = CreateList(0);

	Vector2 playerSize = {0};

	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];
		SDL_Texture *texture = LoadTexture("D:/CWindowGame/Assets/SlimeEnemy.png", window->renderer);
		SDL_QueryTexture(texture, NULL, NULL, &playerSize.x, &playerSize.y);
		AddToList(player->texturesList, texture);
	}

	playerSize.x *= scale.x;
	playerSize.y *= scale.y;

	player->worldPosition = position;
	player->scale = scale;
	player->size = playerSize;

	AddToDrawList(app->drawList, player);

	return player;
}


void MovePlayer(App *app, Entity *player)
{
	Vector2 direction = GetMoveDirection();

	if (direction.x == 0 && direction.y == 0)
	{
		return;
	}

	Vector2 nextFramePosition = {
		player->worldPosition.x + direction.x * MOVE_SPEED, player->worldPosition.y + direction.y * MOVE_SPEED
	};

	if (IsPositionInBounds(app, player, nextFramePosition))
	{
		player->worldPosition = nextFramePosition;
	}
}
