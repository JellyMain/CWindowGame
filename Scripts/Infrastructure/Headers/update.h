#pragma once
#include "../../General/Headers/structs.h"


UpdateSystem *CreateUpdateSystem();
Updatable *CreateUpdatable(void *data, void (*Update)(void *self, App *app, float deltaTime));
void DestroyUpdatable(Updatable *updatable);
