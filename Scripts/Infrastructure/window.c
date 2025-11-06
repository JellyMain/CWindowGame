#include "Headers/window.h"

#include "Headers/update.h"
#include "../UI/Headers/ui.h"
#include "../Utils/Headers/mathUtils.h"


Window *GetWindowById(App *app, uint32_t id)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = ListGet(app->allWindows, i);

		if (SDL_GetWindowID(window->sdlWindow) == id)
		{
			return window;
		}
	}

	SDL_Log("Window with id %d not found", id);
	return NULL;
}


void SetFocusWindow(App *app, Window *window)
{
	app->focusedWindow = window;
	SDL_RaiseWindow(window->sdlWindow);
}


Window *CreateGameWindowWithRenderer(App *app, Vector2Int position, Vector2Int size,
                                     WindowRenderType renderType, WindowType windowType, char *title)
{
	Uint32 windowFlags = SDL_WINDOW_SHOWN;

	if (windowType != FIXED_SIZE)
	{
		windowFlags |= SDL_WINDOW_RESIZABLE;
	}

	windowFlags |= SDL_WINDOW_OPENGL;

	SDL_Window *sdlWindow = SDL_CreateWindow(title, position.x, position.y, size.x, size.y,
	                                         windowFlags);

	Window *window = calloc(1, sizeof(Window));

	SDL_Renderer *renderer = SDL_CreateRenderer(sdlWindow, -1,
	                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Vector2Float windowCenterPoint = {size.x / 2, size.y / 2};
	Vector2Float windowUpperLeftPoint = {0, 0};
	Vector2Float windowLowerLeftPoint = {0, size.y};
	Vector2Float windowUpperRightPoint = {size.x, 0};
	Vector2Float windowLowerRightPoint = {size.x, size.y};

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
	window->gameEntitiesDrawList = ListCreate(0);
	window->uiEntitiesDrawList = ListCreate(0);
	window->gizmosEntitiesDrawList = ListCreate(0);

	ListAdd(app->allWindows, window);

	SetFocusWindow(app, window);

	return window;
}


void UpdateWindow(App *app, Window *window)
{
	ListClear(window->entitiesInWindowList);

	for
	(
		int i = 0;
		i < app->allGameEntities->size;
		i
		++
	)
	{
		GameEntity *entity = app->allGameEntities->elements[i];

		Vector2Float minBounds = {window->position.x, window->position.y};
		Vector2Float maxBounds = {
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


void UpdateWindows(void *data, App *app, float deltaTime)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];
		UpdateWindow(app, window);
	}
}


Updatable *CreateWindowsUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateWindows);
	return updatable;
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
