#include "Headers/winService.h"
#include <SDL_ttf.h>

#include "Headers/ui.h"
#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/window.h"
#include "Utils/mathUtils.h"
#include "Utils/tweener.h"

void OnButtonClicked();

void OnButtonClickedAnimation(App *app, UIEntity *uiEntity);

void CreateWinScreen(App *app, int fontSize)
{
	Window *winWindow = CreateGameWindowWithRenderer(app, (Vector2Int){600, 600}, (Vector2Int){600, 600},
	                                                 WINDOW_SCREEN_SPACE,
	                                                 FIXED_SIZE,
	                                                 "WinWindow");

	Vector2Int youWinTextPosition = (Vector2Int){
		winWindow->windowSize.x / 2,
		winWindow->windowSize.y / 2
	};


	Vector2Int continueTextPosition = (Vector2Int){winWindow->windowSize.x / 2, winWindow->windowSize.y / 2 + 100};

	CreateStaticText("You Win!!!!!!!!!", fontSize, (SDL_Color){255, 255, 255}, app, winWindow, youWinTextPosition,
	                 (Vector2Float){1, 1}, NULL);


	CreateButton(NULL, (Vector2Int){200, 100}, (SDL_Color){255, 255, 255}, "Continue", fontSize,
	             (SDL_Color){0, 0, 0}, app, winWindow,
	             continueTextPosition, (Vector2Float){1, 1}, OnButtonClicked, OnButtonClickedAnimation,NULL);
}


void OnButtonClicked()
{
	printf("Button clicked\n");
}


void OnButtonClickedAnimation(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.target = &uiEntity->scale,
			.fromValue = uiEntity->scale,
			.endValue = {2, 2}
		}
	};

	CreateTween(app, VECTOR2_FLOAT_TWEEN, tweenData, 2, IN_OUT_BOUNCE);
}
