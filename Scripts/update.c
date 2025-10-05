#include "Headers/update.h"

#include "Headers/structs.h"


UpdateSystem *CreateUpdateSystem()
{
	UpdateSystem *updateSystem = calloc(1, sizeof(UpdateSystem));
	updateSystem->updatables = ListCreate(0);
	return updateSystem;
}


Updatable *CreateUpdatable(void *data, void (*Update)(void *self, App *app, float deltaTime))
{
	Updatable *updatable = calloc(1, sizeof(Updatable));
	updatable->data = data;
	updatable->Update = Update;
	return updatable;
}


void DestroyUpdatable(Updatable *updatable)
{
	if (updatable == NULL)
	{
		return;
	}

	free(updatable);
}
