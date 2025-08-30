#pragma once
#include <SDL_render.h>

#include "app.h"
#include "structs.h"
#include "window.h"

GameEntity *CreatePlayer(App *app, Vector2Int position, Vector2Float scale);

void MovePlayer(App *app, GameEntity *player);

