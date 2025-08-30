#include "Headers/window.h"

#include "Headers/app.h"
#include "Headers/input.h"
#include "Headers/ui.h"
#include "Utils/MathUtils.h"


Window *CreateGameWindowWithRenderer(App *app, Vector2Int position, Vector2Int size, WindowRenderType renderType,
                                     WindowType windowType,
                                     char *title)
{
	SDL_Window *sdlWindow = SDL_CreateWindow(title, position.x, position.y, size.x, size.y,
	                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	Window *outWindow = calloc(1, sizeof(Window));

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

	List *gameEntitiesDrawList = CreateList(0);
	List *uiEntitiesDrawList = CreateList(0);
	List *gizmosEntitiesDrawList = CreateList(0);

	AddToDictionary(app->gameEntitiesDrawDictionary, outWindow, gameEntitiesDrawList);
	AddToDictionary(app->uiEntitiesDrawDictionary, outWindow, uiEntitiesDrawList);
	AddToDictionary(app->gizmosEntitiesDrawDictionary, outWindow, gizmosEntitiesDrawList);

	AddAtlasTexture(app->textAtlas, outWindow);

	return outWindow;
}


void UpdateWindow(App *app, Window *window)
{
	ClearList(window->entitiesInWindowList);
	List *drawList = GetFromDictionary(app->gameEntitiesDrawDictionary, window);

	for (int i = 0; i < drawList->size; i++)
	{
		GameEntity *entity = drawList->elements[i];

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
			GameEntity *entity = window->entitiesInWindowList->elements[i];

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


unsigned int HashWindow(void *window)
{
	if (window == NULL)
	{
		return 0;
	}

	Window *windowPtr = window;

	unsigned int hash = 5381;

	hash = (hash << 5) + hash + (unsigned int) (uintptr_t) windowPtr->sdlWindow;

	hash = (hash << 5) + hash + (unsigned int) windowPtr->renderType;
	hash = (hash << 5) + hash + (unsigned int) windowPtr->windowType;

	hash = (hash << 5) + hash + (unsigned int) (uintptr_t) windowPtr->renderer;


	return hash;
}


bool WindowEquals(void *window1, void *window2)
{
	if (window1 == NULL || window2 == NULL)
	{
		return false;
	}

	Window *window1Ptr = window1;
	Window *window2Ptr = window2;

	return window1Ptr == window2Ptr;
}
