#include <SDL_events.h>
#include <SDL_timer.h>
#include <time.h>

#include "Headers/stateMachine.h"
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
			UpdateRenderer(NULL, g_app, 0);
			UpdateWindows(NULL, g_app, 0);
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
	App *app = CreateApp(4, true);

	if (InitSDL2(app) != 0)
	{
		return 1;
	}

	g_app = app;

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

		for (int i = 0; i < app->updateSystem->updatables->size; i++)
		{
			Updatable *updatable = app->updateSystem->updatables->elements[i];
			updatable->Update(updatable->data, app, deltaTime);
		}

		SDL_Delay(16);
	}

	return 0;
}
