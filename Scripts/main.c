#include <SDL_events.h>
#include <SDL_timer.h>

#include "Headers/levelTarget.h"
#include "Headers/player.h"
#include "Headers/draw.h"
#include "Headers/initSDL2.h"
#include "Headers/structs.h"
#include "Headers/app.h"
#include "Headers/input.h"
#include "Headers/winService.h"


static App *g_app = NULL;

int EventFilter(void *userdata, SDL_Event *event)
{
	if (event->type == SDL_SYSWMEVENT || event->type == SDL_WINDOWEVENT)
	{
		if (g_app)
		{
			Render(g_app);

			for (int i = 0; i < g_app->windowsList->size; i++)
			{
				Window *window = g_app->windowsList->elements[i];
				UpdateWindow(g_app, window);
			}
		}
	}

	return 1;
}


void CleanUpWindow(Window *window)
{
	if (window == NULL)
	{
		return;
	}

	SDL_DestroyWindow(window->sdlWindow);
	SDL_DestroyRenderer(window->renderer);

	free(window);
}


void CleanUpEntity(Entity *entity)
{
	if (entity == NULL)
	{
		return;
	}

	for (int i = 0; i < entity->texturesList->size; ++i)
	{
		SDL_DestroyTexture(entity->texturesList->elements[i]);
	}

	DestroyList(entity->texturesList);
	free(entity);
}


void CleanUpLevel(App *app)
{
	for (int i = 0; i < app->drawList->size; ++i)
	{
		Entity *entity = app->drawList->elements[i];

		CleanUpEntity(entity);
	}

	for (int i = 0; i < app->windowsList->size; i++)
	{
		Window *window = app->windowsList->elements[i];
		CleanUpWindow(window);
	}

	ClearList(app->drawList);
	ClearList(app->windowsList);
}


int main()
{
	App *app = CreateApp();

	if (InitSDL2(app) != 0)
	{
		return 1;
	}

	Entity *player = CreatePlayer(app, (Vector2Int){100, 100}, (Vector2Int){2, 2});
	Entity *levelTarget = CreateLevelTarget(app, (Vector2Int){500, 500}, (Vector2Int){2, 2});

	g_app = app;

	SDL_SetEventFilter(EventFilter, NULL);

	while (1)
	{
		Render(app);

		for (int i = 0; i < app->windowsList->size; i++)
		{
			Window *window = app->windowsList->elements[i];
			UpdateWindow(app, window);
		}

		ProcessInput();

		MovePlayer(app, player);

		if (HasReachedLevelTarget(player, levelTarget))
		{
			if (app->hasWon == false)
			{
				SDL_SetEventFilter(NULL, NULL);
				CleanUpLevel(app);
				CreateWinScreen(app, 64);
				app->hasWon = true;
			}
		}

		SDL_Delay(16);
	}

	return 0;
}
