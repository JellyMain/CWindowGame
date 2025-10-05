#pragma once
#include <SDL_render.h>

#include "structs.h"
#include "../DataStructures/Headers/dictionary.h"

SDL_Texture *LoadTexture(char *fileName, SDL_Renderer *renderer);

void Render(App *app);

Dictionary *InitDrawDictionary();

void AddToAllGameEntitiesDrawLists(App *app, GameEntity *entity);

void AddToGameEntitiesDrawList(App *app, Window *window, GameEntity *entity);

void AddToUIEntitiesDrawList(App *app, Window *window, UIEntity *entity);

void AddToGizmoEntitiesDrawList(App *app, Window *window, GizmoEntity *gizmoEntity);
