#include "Headers/app.h"
#include "Headers/draw.h"


App *CreateApp()
{
	App *app = malloc(sizeof(App));
	app->hasWon = false;
	app->drawDictionary = InitDrawDictionary();
	app->allEntities = CreateList(0);

	return app;
}
