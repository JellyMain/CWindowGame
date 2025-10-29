#include "Headers/stateMachine.h"

#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/menu.h"
#include "Headers/levelService.h"
#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/structs.h"
#include "Headers/ui.h"
#include "Headers/window.h"
#include "Headers/winService.h"
#include "Utils/tweener.h"


void AddUpdatable(App *app, Updatable *updatable)
{
	ListAdd(app->updateSystem->updatables, updatable);
}


void AddDefaultUpdatables(App *app)
{
	AddUpdatable(app, CreateRenderUpdatable());
	AddUpdatable(app, CreateInputUpdatable());
	AddUpdatable(app, CreateWindowsUpdatable());
	AddUpdatable(app, CreateTweenersUpdatable());
	AddUpdatable(app, CreateUIUpdatable());
}


void SetPendingState(App *app, GameState state)
{
	if (app->pendingGameState != NONE_GAME_STATE)
	{
		printf("There is already a pending state");
		return;
	}

	app->pendingGameState = state;
}


void FillUpdateLoop(App *app, GameState state)
{
	switch (state)
	{
		case MENU_GAME_STATE:
			AddDefaultUpdatables(app);
			break;
		case GAMEPLAY_GAME_STATE:
			AddDefaultUpdatables(app);
			AddUpdatable(app, CreatePlayerUpdatable(app->levelData->player));
			AddUpdatable(app, CreateLevelTargetUpdatable());
			break;
		case GAME_OVER_GAME_STATE:
			AddDefaultUpdatables(app);
			break;

		default:
			break;
	}
}


void EnterState(App *app, GameState state)
{
	SDL_SetEventFilter(NULL, NULL);
	switch (state)
	{
		case MENU_GAME_STATE:
			CreateMenu(app);
			break;
		case GAMEPLAY_GAME_STATE:
			CleanUpScene(app);
			CreateLevel(app, 0);
			break;
		case GAME_OVER_GAME_STATE:
			CleanUpScene(app);
			CreateWinScreen(app);
			break;
		default:
			break;
	}

	app->gameState = state;

	FillUpdateLoop(app, state);
}
