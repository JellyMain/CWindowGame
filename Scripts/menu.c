#include "Headers/menu.h"

#include "Headers/draw.h"
#include "Headers/stateMachine.h"
#include "Headers/levelService.h"
#include "Headers/structs.h"
#include "Headers/ui.h"
#include "Headers/window.h"
#include "Utils/tweener.h"

void PlayButtonInteraction(App *app, void *data);
void OnPlayButtonHover(App *app, UIEntity *uiEntity);
void OnPlayButtonHoverExit(App *app, UIEntity *uiEntity);


void CreateMenu(App *app)
{
	Window *menuWindow = CreateGameWindowWithRenderer(app, GetDisplayCenterPosition(),
	                                                  (Vector2Int){500, 600}, WINDOW_SCREEN_SPACE,
	                                                  FIXED_SIZE, "Menu");


	SDL_SetWindowPosition(menuWindow->sdlWindow, menuWindow->position.x - menuWindow->size.x / 2,
	                      menuWindow->position.y - menuWindow->size.y / 2);


	SDL_Texture *playButtonTexture = LoadTexture("ButtonSprite.png", menuWindow->renderer);


	CreateButton(playButtonTexture, (Vector2Int){100, 50}, (SDL_Color){0, 0, 0}, "Play", (Vector2Float){0.3f, 0.3f},
	             (SDL_Color){0, 0, 0}, app, menuWindow, menuWindow->windowCenterPoint,
	             (Vector2Float){2, 2}, NULL, PlayButtonInteraction, NULL, OnPlayButtonHover, OnPlayButtonHoverExit, NULL);
}


void PlayButtonInteraction(App *app, void *data)
{
	SetPendingState(app, GAMEPLAY_GAME_STATE);
}


void OnPlayButtonHoverExit(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1.5f, 1.5f},
			.endValue = {1, 1}
		}
	};


	Tween *playButtonTween = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale, tweenData, 0.5f, true, OUT_QUINT);
	PlayTween(app, playButtonTween);
}


void OnPlayButtonHover(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1, 1},
			.endValue = {1.5f, 1.5f}
		}
	};


	Tween *playButtonTween = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale, tweenData, 0.5f, true, OUT_QUINT);
	PlayTween(app, playButtonTween);
}
