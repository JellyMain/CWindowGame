#include <SDL_events.h>
#include <SDL_timer.h>
#include <time.h>
#include "Infrastructure/Headers/stateMachine.h"
#include "Render/Headers/draw.h"
#include "Infrastructure/Headers/init.h"
#include "General/Headers/structs.h"
#include "Infrastructure/Headers/app.h"
#include "Infrastructure/Headers/window.h"


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
	App *app = CreateApp(2, true);

	Init(app);

	g_app = app;

	SetPendingState(app, MENU_GAME_STATE);

	Uint64 lastFrameTime = SDL_GetPerformanceCounter();
	Uint64 currentFrameTime = 0;
	float deltaTime = 0.0f;

	while (1)
	{
		HandleGameStates(app);

		app->time = SDL_GetTicks() / 1000.0f;

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
