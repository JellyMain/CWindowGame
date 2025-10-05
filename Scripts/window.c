#include "Headers/window.h"

#include "Headers/app.h"
#include "Headers/input.h"
#include "Headers/ui.h"
#include "Utils/mathUtils.h"


Window *CreateGameWindowWithRenderer(App *app, Vector2Int position, Vector2Int size, WindowRenderType renderType,
                                     WindowType windowType,
                                     char *title)
{
	Uint32 windowFlags = SDL_WINDOW_SHOWN;

	if (windowType != FIXED_SIZE)
	{
		windowFlags |= SDL_WINDOW_RESIZABLE;
	}


	SDL_Window *sdlWindow = SDL_CreateWindow(title, position.x, position.y, size.x, size.y,
	                                         windowFlags);

	Window *window = calloc(1, sizeof(Window));

	SDL_Renderer *renderer = SDL_CreateRenderer(sdlWindow, -1,
	                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Vector2Int windowCenterPoint = (Vector2Int){size.x / 2, size.y / 2};
	Vector2Int windowUpperLeftPoint = (Vector2Int){0, 0};
	Vector2Int windowLowerLeftPoint = (Vector2Int){0, size.y};
	Vector2Int windowUpperRightPoint = (Vector2Int){size.x, 0};
	Vector2Int windowLowerRightPoint = (Vector2Int){size.x, size.y};

	window->sdlWindow = sdlWindow;
	window->renderer = renderer;
	window->position = position;
	window->windowCenterPoint = windowCenterPoint;
	window->upperRightPoint = windowUpperRightPoint;
	window->lowerRightPoint = windowLowerRightPoint;
	window->upperLeftPoint = windowUpperLeftPoint;
	window->lowerLeftPoint = windowLowerLeftPoint;
	window->size = size;
	window->renderType = renderType;
	window->windowType = windowType;
	window->lastFrameSize = size;
	window->entitiesInWindowList = ListCreate(0);

	List *gameEntitiesDrawList = ListCreate(0);
	List *uiEntitiesDrawList = ListCreate(0);
	List *gizmosEntitiesDrawList = ListCreate(0);

	DictionaryAdd(app->gameEntitiesDrawDictionary, window, gameEntitiesDrawList);
	DictionaryAdd(app->uiEntitiesDrawDictionary, window, uiEntitiesDrawList);
	DictionaryAdd(app->gizmosEntitiesDrawDictionary, window, gizmosEntitiesDrawList);

	AddAtlasTexture(app->textAtlas, window);

	return window;
}


void UpdateWindow(App *app, Window *window)
{
	ListClear(window->entitiesInWindowList);
	List *drawList = DictionaryGet(app->gameEntitiesDrawDictionary, window);

	for (int i = 0; i < drawList->size; i++)
	{
		GameEntity *entity = drawList->elements[i];

		Vector2Int minBounds = {window->position.x, window->position.y};
		Vector2Int maxBounds = {
			window->position.x + window->size.x, window->position.y + window->size.y
		};

		if (IsEntityInBounds(entity, entity->worldPosition, minBounds, maxBounds))
		{
			ListAdd(window->entitiesInWindowList, entity);
		}
	}


	SDL_GetWindowPosition(window->sdlWindow, &window->position.x, &window->position.y);
	window->viewportOffset = window->position;


	SDL_GetWindowSize(window->sdlWindow, &window->size.x, &window->size.y);

	Vector2Float resizePercentDelta = GetPercentageChangeVector2(
		(Vector2Float){window->lastFrameSize.x, window->lastFrameSize.y},
		(Vector2Float){window->size.x, window->size.y});


	window->lastFrameSize = window->size;

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


Vector2Int GetDisplayCenterPosition()
{
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	int centerX = displayMode.w / 2;
	int centerY = displayMode.h / 2;

	return (Vector2Int){centerX, centerY};
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
