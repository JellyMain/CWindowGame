#include "../Services/Headers/winService.h"
#include <SDL_ttf.h>

#include "../UI/Headers/ui.h"
#include "../Infrastructure/Headers/window.h"
#include "../Tween/Headers/tweener.h"

void OnButtonClicked(App *app, void *data);

void OnButtonClickedAnimation(App *app, UIEntity *uiEntity);


void CreateWinScreen(App *app)
{
	Window *winWindow = CreateGameWindowWithRenderer(app, (Vector2Int){600, 600}, (Vector2Int){600, 600},
	                                                 WINDOW_SCREEN_SPACE, FIXED_SIZE, "WinWindow"
	);

	Vector2Int youWinTextPosition = (Vector2Int)
	{
		winWindow->size.x / 2,
		winWindow->size.y / 2
	};


	Vector2Int continueTextPosition = (Vector2Int)
	{
		winWindow->size.x / 2, winWindow->size.y / 2 + 120
	};


	CreateStaticText(app, "You Win!!!!!!!!!", (SDL_Color){255, 255, 255}, youWinTextPosition, (Vector2Float){0.3f, 0.3f},
	                 NULL);


	CreateButton(NULL, (Vector2Int){75, 30}, (SDL_Color){255, 255, 255, 255}, "Continue", (Vector2Float){0.3f, 0.3f},
	             (SDL_Color){0, 0, 0}, app, winWindow, continueTextPosition, (Vector2Float){1, 1},NULL, OnButtonClicked,
	             OnButtonClickedAnimation, NULL, NULL, NULL);
}


void OnButtonClicked(App *app, void *data)
{
	printf("Button clicked\n");
}


void OnButtonClickedAnimation(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1, 1},
			.endValue = {2, 2}
		}
	};


	TweenData tweenDataReverse = {
		.vector2FloatTween = {
			.fromValue = {2, 2},
			.endValue = {1, 1}
		}
	};

	Tween *buttonTween = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale, tweenData, 2,
	                                 true, IN_OUT_BOUNCE);
	Tween *buttonTweenReverse = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale,
	                                        tweenDataReverse, 2, true,
	                                        IN_OUT_BOUNCE);

	TweenSequence *buttonTweenSequence = CreateTweenSequence();

	AddTweenToSequence(buttonTweenSequence, buttonTween);
	AddTweenToSequence(buttonTweenSequence, buttonTweenReverse);

	PlayTweenSequence(app, buttonTweenSequence);
}
