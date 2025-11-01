#include "../Infrastructure/Headers/stateMachine.h"

#include "../Render/Headers/draw.h"
#include "../Input/Headers/input.h"
#include "../General/Headers/menu.h"
#include "../Services/Headers/levelService.h"
#include "../General/Headers/levelTarget.h"
#include "../General/Headers/player.h"
#include "../General/Headers/structs.h"
#include "../UI/Headers/ui.h"
#include "../Infrastructure/Headers/window.h"
#include "../Services/Headers/winService.h"
#include "../Tween/Headers/tweener.h"


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
	SDL_GL_MakeCurrent(app->hiddenWindow, app->glContext);

	switch (state)
	{
		case MENU_GAME_STATE:
			CreateMainMenu(app);
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
