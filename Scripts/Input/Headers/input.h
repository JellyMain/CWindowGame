#pragma once
#include "../../General/Headers/structs.h"

Vector2Float GetMoveDirection();

Vector2Float GetMousePosition();

bool IsLeftMouseButtonClicked();

Updatable *CreateInputUpdatable();
