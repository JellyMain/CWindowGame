#include "Headers/initSDL2.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Headers/ui.h"
#include "Headers/window.h"


int InitSDL2(App *app)
{
	if (!app)
	{
		return -1;
	}


	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Sdl video init error");
		return -2;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);


	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init failed: %s", TTF_GetError());
		return -3;
	}


	app->textAtlas = CreateTextAtlas("ByteBounce.ttf", 16);

	return 0;
}
