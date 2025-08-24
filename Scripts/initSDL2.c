#include "Headers/initSDL2.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Headers/app.h"
#include "Headers/window.h"

int InitSDL2(App *outApp)
{
	if (!outApp)
	{
		return -1;
	}


	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Sdl video init error");
		return -2;
	}


	Window *windowA = CreateGameWindowWithRenderer((Vector2){300, 100}, (Vector2){300, 300}, WINDOW_WORLD_SPACE,
	                                               "WindowA");
	Window *windowB = CreateGameWindowWithRenderer((Vector2){100, 100}, (Vector2){300, 300}, WINDOW_WORLD_SPACE,
	                                               "WindowB");
	Window *windowC = CreateGameWindowWithRenderer((Vector2){500, 300}, (Vector2){300, 300}, WINDOW_WORLD_SPACE,
	                                               "WindowC");


	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);


	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init failed: %s", TTF_GetError());
		return -3;
	}

	AddToList(outApp->windowsList, windowA);
	AddToList(outApp->windowsList, windowB);
	AddToList(outApp->windowsList, windowC);

	return 0;
}

