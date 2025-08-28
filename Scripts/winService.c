#include "Headers/winService.h"
#include <SDL_ttf.h>

#include "Headers/text.h"
#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/window.h"


void CreateWinScreen(App *app, int fontSize)
{
	Window *winWindow = CreateGameWindowWithRenderer((Vector2Int){600, 600}, (Vector2Int){600, 600},
	                                                 WINDOW_SCREEN_SPACE,
	                                                 FIXED_SIZE,
	                                                 "WinWindow");

	Window *anotherWindow = CreateGameWindowWithRenderer((Vector2Int){600, 600}, (Vector2Int){600, 600},
	                                                     WINDOW_SCREEN_SPACE,
	                                                     FIXED_SIZE,
	                                                     "AnotherWindow");

	AddToList(app->windowsList, winWindow);
	AddToList(app->windowsList, anotherWindow);

	CreateText("You Win", fontSize, (SDL_Color){255, 255, 255}, app, winWindow, (Vector2Float){1, 1});

	CreateText("You Lose", 64, (SDL_Color){255, 255, 255}, app, anotherWindow, (Vector2Float){1, 1});
}
