#pragma once
#include <SDL_render.h>

#include "app.h"
#include "structs.h"
#include "window.h"

Entity *CreatePlayer(App *app, Vector2 position, Vector2 scale);

void MovePlayer(App *app, Entity *player);

