#pragma once
#include <SDL_render.h>

#include "app.h"
#include "structs.h"
#include "window.h"
#include "../DataStructures/Headers/Dictionary.h"

SDL_Texture *LoadTexture(char *fileName, SDL_Renderer *renderer);

void Render(App *app);

Dictionary *InitDrawDictionary();

void AddToAllDrawLists(App *app, Entity *entity);

void AddToWindowDrawList(App *app, Window *window, Entity *entity);
