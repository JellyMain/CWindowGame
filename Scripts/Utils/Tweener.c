#include "Tweener.h"

#include <SDL_timer.h>

#include "MathUtils.h"


void CreateTween(App *app, TweenType tweenType, TweenData tweenData, float duration)
{
	Tween *tween = calloc(1, sizeof(Tween));
	tween->tweenType = tweenType;
	tween->tweenData = tweenData;
	tween->duration = duration;
	tween->elapsedTime = 0;

	AddToList(app->allTweeners, tween);
}


void UpdateTweeners(App *app, float deltaTime)
{
	for (int i = app->allTweeners->size - 1; i >= 0; i--)
	{
		Tween *tween = app->allTweeners->elements[i];

		tween->elapsedTime += deltaTime;
		float t = tween->elapsedTime / tween->duration;

		if (t >= 1.0f)
		{
			switch (tween->tweenType)
			{
				case VECTOR2_FLOAT_TWEEN:
					*tween->tweenData.vector2FloatTween.target = tween->tweenData.vector2FloatTween.endValue;
					break;
				case FLOAT_TWEEN:
					*tween->tweenData.floatTween.target = tween->tweenData.floatTween.endValue;
					break;
			}

			RemoveFromListAtIndex(app->allTweeners, i);
			DestroyTween(tween);
		}
		else
		{
			switch (tween->tweenType)
			{
				case VECTOR2_FLOAT_TWEEN:
					*tween->tweenData.vector2FloatTween.target = LerpVector2Float(
						tween->tweenData.vector2FloatTween.fromValue, tween->tweenData.vector2FloatTween.endValue, t);
					break;

				case FLOAT_TWEEN:
					*tween->tweenData.floatTween.target = LerpFloat(
						tween->tweenData.floatTween.fromValue, tween->tweenData.floatTween.endValue, t);
			}
		}
	}
}


void TweenVector2Float(Vector2Float *from, Vector2Float to, float duration)
{
	Vector2Float originalValue = *from;
	float elapsedTime = 0;
	Uint64 lastFrameTime = SDL_GetPerformanceCounter();
	Uint64 currentFrameTime = 0;
	float deltaTime = 0.0f;
	float t = 0.0f;


	while (elapsedTime < duration)
	{
		currentFrameTime = SDL_GetPerformanceCounter();
		deltaTime = (currentFrameTime - lastFrameTime) / (float) SDL_GetPerformanceFrequency();
		lastFrameTime = currentFrameTime;
		elapsedTime += deltaTime;

		printf("%f\n", deltaTime);

		t = elapsedTime / duration;
		t = ClampFloat(t, 0.0f, 1.0f);

		*from = LerpVector2Float(originalValue, to, t);
		SDL_Delay(16);
	}

	*from = to;
}


void DestroyTween(Tween *tween)
{
	if (tween == NULL)
	{
		return;
	}

	free(tween);
}
