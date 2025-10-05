#include <SDL_events.h>
#include <SDL_timer.h>
#include <time.h>

#include "stateMachine.h"
#include "Headers/ui.h"
#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/draw.h"
#include "Headers/initSDL2.h"
#include "Headers/structs.h"
#include "Headers/app.h"
#include "Headers/input.h"
#include "Headers/levelService.h"
#include "Headers/window.h"
#include "Headers/winService.h"
#include "Utils/tweener.h"


static App *g_app = NULL;

int EventFilter(void *userdata, SDL_Event *event)
{
	if (event->type == SDL_SYSWMEVENT || event->type == SDL_WINDOWEVENT)
	{
		if (g_app)
		{
			Render(g_app);

			for (int i = 0; i < g_app->gameEntitiesDrawDictionary->allPairs->size; i++)
			{
				KeyValuePair *pair = g_app->gameEntitiesDrawDictionary->allPairs->elements[i];
				Window *window = pair->key;
				UpdateWindow(g_app, window);
			}
		}
	}

	return 1;
}


void HandleGameStates(App *app)
{
	if (app->pendingGameState != NONE_GAME_STATE)
	{
		SDL_SetEventFilter(NULL, NULL);
		EnterState(app, app->pendingGameState);
		app->pendingGameState = NONE_GAME_STATE;
		SDL_SetEventFilter(EventFilter, NULL);
	}
}

int main()
{
	srand(time(NULL));
	App *app = CreateApp(true);

	if (InitSDL2(app) != 0)
	{
		return 1;
	}

	g_app = app;


	SDL_SetEventFilter(EventFilter, NULL);


	SetPendingState(app, MENU_GAME_STATE);

	Uint64 lastFrameTime = SDL_GetPerformanceCounter();
	Uint64 currentFrameTime = 0;
	float deltaTime = 0.0f;

	while (1)
	{
		HandleGameStates(app);


		currentFrameTime = SDL_GetPerformanceCounter();
		deltaTime = (currentFrameTime - lastFrameTime) / (float) SDL_GetPerformanceFrequency();
		lastFrameTime = currentFrameTime;

		Render(app);

		UpdateTweeners(app, deltaTime);

		for (int i = 0; i < app->gameEntitiesDrawDictionary->allPairs->size; i++)
		{
			KeyValuePair *pair = app->gameEntitiesDrawDictionary->allPairs->elements[i];
			Window *window = pair->key;
			UpdateWindow(app, window);
		}

		UpdateUIElements(app);

		ProcessInput();

		if (app->gameState == GAMEPLAY_GAME_STATE)
		{
			MovePlayer(app, app->player);

			if (app->hasWon == false)
			{
				if (HasReachedLevelTarget(app->player, app->levelTarget))
				{
					app->hasWon = true;
					SetPendingState(app, GAME_OVER_GAME_STATE);
				}
			}
		}

		SDL_Delay(16);
	}

	return 0;
}
