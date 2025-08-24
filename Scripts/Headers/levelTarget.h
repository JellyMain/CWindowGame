#pragma once
#include "app.h"
#include "structs.h"

Entity *CreateLevelTarget(App *app, Vector2 position, Vector2 scale);

bool HasReachedLevelTarget(Entity *player, Entity *levelTarget);

