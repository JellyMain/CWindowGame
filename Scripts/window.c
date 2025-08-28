#include "Headers/window.h"

#include "Headers/app.h"
#include "Headers/input.h"
#include "Utils/MathUtils.h"


Window *CreateGameWindowWithRenderer(Vector2Int position, Vector2Int size, WindowRenderType renderType,
                                     WindowType windowType,
                                     char *title)
{
	SDL_Window *sdlWindow = SDL_CreateWindow(title, position.x, position.y, size.x, size.y,
	                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	Window *outWindow = malloc(sizeof(Window));

	SDL_Renderer *renderer = SDL_CreateRenderer(sdlWindow, -1,
	                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	outWindow->sdlWindow = sdlWindow;
	outWindow->renderer = renderer;
	outWindow->windowPosition = position;
	outWindow->windowSize = size;
	outWindow->renderType = renderType;
	outWindow->windowType = windowType;
	outWindow->lastFrameSize = size;
	outWindow->entitiesInWindowList = CreateList(0);
	return outWindow;
}


void UpdateWindow(App *app, Window *window)
{
	ClearList(window->entitiesInWindowList);

	for (int i = 0; i < app->drawList->size; ++i)
	{
		Entity *entity = app->drawList->elements[i];

		Vector2Int minBounds = {window->windowPosition.x, window->windowPosition.y};
		Vector2Int maxBounds = {
			window->windowPosition.x + window->windowSize.x, window->windowPosition.y + window->windowSize.y
		};

		if (IsEntityInBounds(entity, entity->worldPosition, minBounds, maxBounds))
		{
			AddToList(window->entitiesInWindowList, entity);
		}
	}


	SDL_GetWindowPosition(window->sdlWindow, &window->windowPosition.x, &window->windowPosition.y);
	window->viewportOffset = window->windowPosition;


	SDL_GetWindowSize(window->sdlWindow, &window->windowSize.x, &window->windowSize.y);

	Vector2Float resizePercentDelta = GetPercentageChangeVector2(
		(Vector2Float){window->lastFrameSize.x, window->lastFrameSize.y},
		(Vector2Float){window->windowSize.x, window->windowSize.y});


	window->lastFrameSize = window->windowSize;

	if (window->windowType == SCALE_WITH_RESIZE)
	{
		for (int i = 0; i < window->entitiesInWindowList->size; i++)
		{
			Entity *entity = window->entitiesInWindowList->elements[i];

			if (resizePercentDelta.x != 0.0f)
			{
				entity->scale.x = entity->scale.x * (1.0f + resizePercentDelta.x / 100.0f);
			}

			if (resizePercentDelta.y != 0.0f)
			{
				entity->scale.y = entity->scale.y * (1.0f + resizePercentDelta.y / 100.0f);
			}
		}
	}
}
