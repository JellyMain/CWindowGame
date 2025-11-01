#pragma once
#include "structs.h"

GameEntity *CreateLevelTarget(App *app, Vector2Int position, Vector2Float scale);

Updatable *CreateLevelTargetUpdatable();
