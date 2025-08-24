#pragma once
#include "structs.h"
#include <SDL_video.h>

typedef enum
{
	WINDOW_SCREEN_SPACE,
	WINDOW_WORLD_SPACE,
} WindowRenderType;

typedef struct
{
	SDL_Window *sdlWindow;
	Vector2 windowPosition;
	Vector2 windowSize;
	Vector2 viewportOffset;
	SDL_Renderer *renderer;
	WindowRenderType type;
} Window;

Window *CreateGameWindowWithRenderer(Vector2 position, Vector2 size, WindowRenderType type, char *title);

void UpdateWindow(Window *window);
