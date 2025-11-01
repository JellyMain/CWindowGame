#include <stdbool.h>
#include "../Infrastructure/Headers/app.h"

#include "../Infrastructure/Headers/update.h"
#include "../Render/Headers/draw.h"
#include "../Services/Headers/levelService.h"


App *CreateApp(int pixelsPerUnit,bool showGizmos)
{
	App *app = calloc(1, sizeof(App));

	app->updateSystem = CreateUpdateSystem();
	app->levelData = CreateLevelData();
	app->pendingGameState = NONE_GAME_STATE;
	app->gameState = MENU_GAME_STATE;
	app->pixelsPerUnit = pixelsPerUnit;
	app->hasWon = false;
	app->showGizmos = showGizmos;
	app->allGizmosEntities = ListCreate(0);
	app->allUIEntities = ListCreate(0);
	app->allGameEntities = ListCreate(0);
	app->allTweeners = ListCreate(0);
	app->allTweenSequences = ListCreate(0);
	app->allWindows = ListCreate(0);
	app->tweenTargetsDictionary = DictionaryCreate(HashPointer, PointerEquals);

	return app;
}
