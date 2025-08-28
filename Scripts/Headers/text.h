#pragma once
#include "structs.h"
#include "window.h"

Entity *CreateText(char *text, int fontSize, SDL_Color textColor, App *app, Window *window, Vector2Float scale);
