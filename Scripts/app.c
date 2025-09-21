#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/ui.h"


App *CreateApp(bool showGizmos)
{
	App *app = calloc(1, sizeof(App));
	app->hasWon = false;
	app->showGizmos = showGizmos;
	app->gameEntitiesDrawDictionary = InitDrawDictionary();
	app->uiEntitiesDrawDictionary = InitDrawDictionary();
	app->gizmosEntitiesDrawDictionary = InitDrawDictionary();
	app->allGizmosEntities = CreateList(0);
	app->allUIEntities = CreateList(0);
	app->allGameEntities = CreateList(0);
	app->allTweeners = CreateList(0);

	return app;
}
