#include "Headers/window.h"

#include "Headers/app.h"
#include "Headers/input.h"
#include "Utils/MathUtils.h"


Window *CreateGameWindowWithRenderer(Vector2 position, Vector2 size, WindowRenderType renderType, WindowType windowType,
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

		Vector2 minBounds = {window->windowPosition.x, window->windowPosition.y};
		Vector2 maxBounds = {
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

	Vector2 resizePercentDelta = GetPercentageChangeVector2((Vector2){window->lastFrameSize.x, window->lastFrameSize.y},
	                                                        (Vector2){window->windowSize.x, window->windowSize.y});

	window->lastFrameSize = window->windowSize;

	if (window->windowType == SCALE_WITH_RESIZE)
	{
		for (int i = 0; i < window->entitiesInWindowList->size; i++)
		{
			Entity *entity = window->entitiesInWindowList->elements[i];
			entity->size.x *= resizePercentDelta.x / 100;
			entity->size.y *= resizePercentDelta.y / 100;

			printf("Resize: %d %d\n", resizePercentDelta.x, resizePercentDelta.y);
		}
	}
}
