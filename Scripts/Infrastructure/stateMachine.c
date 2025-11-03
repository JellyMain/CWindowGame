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


void EnterState(App *app, GameState state)
{
	SDL_SetEventFilter(NULL, NULL);
	SDL_GL_MakeCurrent(app->hiddenWindow, app->glContext);

	CleanUpScene(app);
	AddDefaultUpdatables(app);

	switch (state)
	{
		case MENU_GAME_STATE:
			CreateMainMenu(app);
			break;
		case GAMEPLAY_GAME_STATE:
			CreateLevel(app, 0);
			break;
		case GAME_OVER_GAME_STATE:
			CreateWinScreen(app);
			break;
		default:
			break;
	}

	app->gameState = state;
}
