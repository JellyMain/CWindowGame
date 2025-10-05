#include "stateMachine.h"

#include "menu.h"
#include "Headers/levelService.h"
#include "Headers/structs.h"
#include "Headers/winService.h"


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
			CreateMenu(app);
			break;
		case GAMEPLAY_GAME_STATE:

			CleanUpLevel(app);
			CreateLevel(app, 0);
			break;
		case GAME_OVER_GAME_STATE:
			CleanUpLevel(app);
			CreateWinScreen(app);
			break;
		default:
			break;
	}


	app->gameState = state;
}
