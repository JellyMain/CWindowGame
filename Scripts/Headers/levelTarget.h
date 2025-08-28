#pragma once
#include "app.h"
#include "structs.h"

Entity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale);

bool HasReachedLevelTarget(Entity *player, Entity *levelTarget);

