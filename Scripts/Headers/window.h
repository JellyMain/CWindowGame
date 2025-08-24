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
	Vector2 windowPosition;
	Vector2 windowSize;
	Vector2 viewportOffset;
	Vector2 lastFrameSize;
	SDL_Renderer *renderer;
	WindowRenderType renderType;
	WindowType windowType;
	List *entitiesInWindowList;
} Window;

Window *CreateGameWindowWithRenderer(Vector2 position, Vector2 size, WindowRenderType renderType, WindowType windowType,
                                     char *title);

void UpdateWindow(App *app, Window *window);
