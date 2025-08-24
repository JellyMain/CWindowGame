#pragma once
#include <SDL_render.h>

#include "app.h"
#include "structs.h"
#include "window.h"

Entity *CreatePlayer(App *app, Vector2Int position, Vector2Int scale);

void MovePlayer(App *app, Entity *player);

