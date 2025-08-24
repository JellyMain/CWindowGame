#pragma once
#include <SDL_render.h>

#include "app.h"
#include "structs.h"
#include "../DataStructures/Headers/List.h"

SDL_Texture *LoadTexture(char *fileName, SDL_Renderer *renderer);

void Render(App *app);

List *InitDrawList();

void AddToDrawList(List *drawList, Entity *entity);
