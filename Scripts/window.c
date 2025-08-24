#include "Headers/window.h"
#include "Headers/input.h"


Window *CreateGameWindowWithRenderer(Vector2 position, Vector2 size, WindowRenderType type, char *title)
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
	outWindow->type = type;
	return outWindow;
}


void UpdateWindow(Window *window)
{
	SDL_GetWindowPosition(window->sdlWindow, &window->windowPosition.x, &window->windowPosition.y);
	window->viewportOffset = window->windowPosition;
}
