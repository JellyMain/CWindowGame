#pragma once
#include "app.h"
#include "structs.h"

GameEntity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale);

bool HasReachedLevelTarget(GameEntity *player, GameEntity *levelTarget);

