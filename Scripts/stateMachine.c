#include "Headers/stateMachine.h"

#include "Headers/menu.h"
#include "Headers/levelService.h"
#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/structs.h"
#include "Headers/ui.h"
#include "Headers/window.h"
#include "Headers/winService.h"
#include "Utils/tweener.h"


void SetPendingState(App *app, GameState state)
{
	if (app->pendingGameState != NONE_GAME_STATE)
	{
		printf("There is already a pending state");
		return;
	}

	app->pendingGameState = state;
}


void EnterState(App *app, GameState state)
{
	SDL_SetEventFilter(NULL, NULL);
	switch (state)
	{
		case MENU_GAME_STATE:
			ListAdd(app->updateSystem->updatables, CreateWindowsUpdatable());
			ListAdd(app->updateSystem->updatables, CreateTweenersUpdatable());
			ListAdd(app->updateSystem->updatables, CreateUIUpdatable());
			CreateMenu(app);
			break;
		case GAMEPLAY_GAME_STATE:
			CleanUpScene(app);
			ListAdd(app->updateSystem->updatables, CreateWindowsUpdatable());
			ListAdd(app->updateSystem->updatables, CreateTweenersUpdatable());
			ListAdd(app->updateSystem->updatables, CreateUIUpdatable());
			CreateLevel(app, 0);
			ListAdd(app->updateSystem->updatables, CreateLevelTargetUpdatable());
			ListAdd(app->updateSystem->updatables, CreatePlayerUpdatable(app->levelData->player));
			break;
		case GAME_OVER_GAME_STATE:
			CleanUpScene(app);
			ListAdd(app->updateSystem->updatables, CreateWindowsUpdatable());
			ListAdd(app->updateSystem->updatables, CreateTweenersUpdatable());
			ListAdd(app->updateSystem->updatables, CreateUIUpdatable());
			CreateWinScreen(app);
			break;
		default:
			break;
	}


	app->gameState = state;
}
