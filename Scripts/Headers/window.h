#pragma once
#include "structs.h"
#include <SDL_video.h>

#include "app.h"

typedef enum
{
	WINDOW_SCREEN_SPACE,
	WINDOW_WORLD_SPACE,
} WindowRenderType;


typedef enum
{
	RESIZABLE,
	FIXED_SIZE,
	SCALE_WITH_RESIZE
} WindowType;

typedef struct
{
	SDL_Window *sdlWindow;
	Vector2Int windowPosition;
	Vector2Int windowSize;
	Vector2Int viewportOffset;
	Vector2Int lastFrameSize;
	SDL_Renderer *renderer;
	WindowRenderType renderType;
	WindowType windowType;
	List *entitiesInWindowList;
} Window;

Window *CreateGameWindowWithRenderer(Vector2Int position, Vector2Int size, WindowRenderType renderType, WindowType windowType,
                                     char *title);

void UpdateWindow(App *app, Window *window);
