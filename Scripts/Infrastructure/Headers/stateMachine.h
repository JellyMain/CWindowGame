#pragma once
#include "General/Headers/structs.h"

void SetPendingState(App *app, GameState state);

void EnterState(App *app, GameState state);

bool HasUpdatable(App *app, Updatable *updatable);

void AddUpdatable(App *app, Updatable *updatable);

void RemoveUpdatable(App *app, Updatable *updatable);
