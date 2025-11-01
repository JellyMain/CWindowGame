#pragma once
#include <SDL_render.h>

#include "../../General/Headers/structs.h"
#include "../../DataStructures/Headers/dictionary.h"


Updatable *CreateRenderUpdatable();

void UpdateRenderer(void *data, App *app, float deltaTime);
