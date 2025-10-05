#pragma once
#include "structs.h"

GameEntity *CreatePlayer(App *app, Vector2Int position, Vector2Float scale);

Updatable *CreatePlayerUpdatable(GameEntity *player);
