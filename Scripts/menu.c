#include "Headers/menu.h"

#include "Headers/stateMachine.h"
#include "Headers/levelService.h"
#include "Headers/structs.h"
#include "Headers/ui.h"
#include "Headers/window.h"

void PlayButtonInteraction(App *app);

void CreateMenu(App *app)
{
	Window *menuWindow = CreateGameWindowWithRenderer(app, GetDisplayCenterPosition(),
	                                                  (Vector2Int){300, 300}, WINDOW_SCREEN_SPACE,
	                                                  FIXED_SIZE, "Menu");


	CreateButton(NULL, (Vector2Int){100, 50}, (SDL_Color){255, 0, 0}, "Play", 32,
	             (SDL_Color){0, 0, 0}, app, menuWindow, menuWindow->windowCenterPoint,
	             VECTOR2_FLOAT_ONE, PlayButtonInteraction, NULL, NULL);
}


void PlayButtonInteraction(App *app)
{
	SetPendingState(app, GAMEPLAY_GAME_STATE);
}
