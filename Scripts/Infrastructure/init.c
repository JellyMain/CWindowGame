#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../Infrastructure/Headers/init.h"
#include "../Render/Headers/openGL.h"


void InitSDL2()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Sdl video init error");
		return;
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);


	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init failed: %s", TTF_GetError());
	}
}


void Init(App *app)
{
	if (!app)
	{
		SDL_Log("App is NULL");
		return;
	}

	InitSDL2();
	InitOpenGL(app);

	// app->textAtlas = CreateTextAtlas("ByteBounce.ttf", 16);
}
