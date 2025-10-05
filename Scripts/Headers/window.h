#pragma once
#include "structs.h"


Window *CreateGameWindowWithRenderer(App *app, Vector2Int position, Vector2Int size,
                                     WindowRenderType renderType,
                                     WindowType windowType,
                                     char *title);

unsigned int HashWindow(void *window);

bool WindowEquals(void *window1, void *window2);

Vector2Int GetDisplayCenterPosition();

Updatable *CreateWindowsUpdatable();
