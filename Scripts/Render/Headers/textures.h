﻿#pragma once
#include <SDL_surface.h>
#include "../../General/Headers/structs.h"

Texture *CreateTextureFromSurface(SDL_Surface *surface);

Texture *LoadTexture(const char *fileName);

Texture *CreateRect(Vector2Int size, SDL_Color color);

Material *CreateMaterial(char *fragShaderName, char *vertShaderName, Texture *texture);
