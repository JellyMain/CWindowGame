#include "Headers/initSDL2.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Headers/ui.h"
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


	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);


	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init failed: %s", TTF_GetError());
		return -3;
	}


	outApp->textAtlas = CreateTextAtlas("D:/CWindowGame/Assets/Merchant Copy.ttf", 16);

	CreateGameWindowWithRenderer(outApp, (Vector2Int){300, 100}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             SCALE_WITH_RESIZE,
	                             "WindowA");
	CreateGameWindowWithRenderer(outApp, (Vector2Int){100, 100}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             FIXED_SIZE,
	                             "WindowB");
	CreateGameWindowWithRenderer(outApp, (Vector2Int){500, 300}, (Vector2Int){300, 300},
	                             WINDOW_WORLD_SPACE,
	                             FIXED_SIZE,
	                             "WindowC");





	return 0;
}
