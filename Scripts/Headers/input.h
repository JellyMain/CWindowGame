#pragma once
#include "structs.h"

Vector2Int GetMoveDirection();

Vector2Int GetMousePosition();

bool IsLeftMouseButtonClicked();

Updatable *CreateInputUpdatable();
