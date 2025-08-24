#include "Headers/app.h"
#include "Headers/draw.h"


App *CreateApp()
{
	App *app = malloc(sizeof(App));
	app->hasWon = false;
	app->windowsList = CreateList(0);
	app->drawList = InitDrawList();

	return app;
}
