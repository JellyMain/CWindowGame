#include "tweener.h"

#include <SDL_timer.h>
#include "mathUtils.h"

float EaseLinear(float t);

float EaseInSine(float t);

float EaseOutSine(float t);

float EaseInOutSine(float t);

float EaseInQuad(float t);

float EaseOutQuad(float t);

float EaseInOutQuad(float t);

float EaseInCubic(float t);

float EaseOutCubic(float t);

float EaseInOutCubic(float t);

float EaseInQuart(float t);

float EaseOutQuart(float t);

float EaseInOutQuart(float t);

float EaseInQuint(float t);

float EaseOutQuint(float t);

float EaseInOutQuint(float t);

float EaseInExpo(float t);

float EaseOutExpo(float t);

float EaseInOutExpo(float t);

float EaseInCirc(float t);

float EaseOutCirc(float t);

float EaseInOutCirc(float t);

float EaseInBack(float t);

float EaseOutBack(float t);

float EaseInOutBack(float t);

float EaseInElastic(float t);

float EaseOutElastic(float t);

float EaseInOutElastic(float t);

float EaseInBounce(float t);

float EaseOutBounce(float t);

float EaseInOutBounce(float t);

void UpdateTween(App *app, Tween *tween, float deltaTime);


Tween *CreateTween(TweenType tweenType, TweenData tweenData, float duration, bool destroyOnComplete,
                   TweenEasingType easingType)
{
	Tween *tween = calloc(1, sizeof(Tween));
	tween->destroyOnComplete = destroyOnComplete;
	tween->isFinished = false;
	tween->tweenType = tweenType;
	tween->tweenData = tweenData;
	tween->duration = duration;
	tween->elapsedTime = 0;
	tween->easingType = easingType;

	return tween;
}


void PlayTween(App *app, Tween *tween)
{
	AddToList(app->allTweeners, tween);
}


TweenSequence *CreateTweenSequence()
{
	TweenSequence *tweenSequence = calloc(1, sizeof(TweenSequence));
	tweenSequence->tweeners = CreateList(0);
	return tweenSequence;
}


void PlayTweenSequence(App *app, TweenSequence *tweenSequence)
{
	tweenSequence->isStarted = true;
	AddToList(app->allTweenSequences, tweenSequence);
}


void AddTweenToSequence(TweenSequence *tweenSequence, Tween *tween)
{
	if (!tweenSequence->isStarted)
	{
		tween->destroyOnComplete = false;
		AddToList(tweenSequence->tweeners, tween);
	}
	else
	{
		printf("Sequence is already started");
	}
}


void UpdateSequences(App *app, float deltaTime)
{
	for (int i = app->allTweenSequences->size - 1; i >= 0; i--)
	{
		TweenSequence *tweenSequence = app->allTweenSequences->elements[i];

		for (int j = 0; j < tweenSequence->tweeners->size; j++)
		{
			Tween *tween = tweenSequence->tweeners->elements[j];

			if (!tween->isFinished)
			{
				UpdateTween(app, tween, deltaTime);
				break;
			}
		}

		Tween *lastTween = tweenSequence->tweeners->elements[tweenSequence->tweeners->size - 1];

		if (lastTween->isFinished)
		{
			RemoveFromList(app->allTweenSequences, tweenSequence);
			DestroySequence(tweenSequence);
		}
	}
}


void UpdateTweeners(App *app, float deltaTime)
{
	UpdateSequences(app, deltaTime);

	for (int i = app->allTweeners->size - 1; i >= 0; i--)
	{
		Tween *tween = app->allTweeners->elements[i];

		UpdateTween(app, tween, deltaTime);
	}
}


void UpdateTween(App *app, Tween *tween, float deltaTime)
{
	if (tween->isFinished)
	{
		return;
	}

	tween->elapsedTime += deltaTime;

	float t = tween->elapsedTime / tween->duration;

	switch (tween->easingType)
	{
		case LINEAR:
			t = EaseLinear(t);
			break;
		case IN_SINE:
			t = EaseInSine(t);
			break;
		case OUT_SINE:
			t = EaseOutSine(t);
			break;
		case IN_OUT_SINE:
			t = EaseInOutSine(t);
			break;
		case IN_QUAD:
			t = EaseInQuad(t);
			break;
		case OUT_QUAD:
			t = EaseOutQuad(t);
			break;
		case IN_OUT_QUAD:
			t = EaseInOutQuad(t);
			break;
		case IN_CUBIC:
			t = EaseInCubic(t);
			break;
		case OUT_CUBIC:
			t = EaseOutCubic(t);
			break;
		case IN_OUT_CUBIC:
			t = EaseInOutCubic(t);
			break;
		case IN_QUART:
			t = EaseInQuart(t);
			break;
		case OUT_QUART:
			t = EaseOutQuart(t);
			break;
		case IN_OUT_QUART:
			t = EaseInOutQuart(t);
			break;
		case IN_QUINT:
			t = EaseInQuint(t);
			break;
		case OUT_QUINT:
			t = EaseOutQuint(t);
			break;
		case IN_OUT_QUINT:
			t = EaseInOutQuint(t);
			break;
		case IN_EXPO:
			t = EaseInExpo(t);
			break;
		case OUT_EXPO:
			t = EaseOutExpo(t);
			break;
		case IN_OUT_EXPO:
			t = EaseInOutExpo(t);
			break;
		case IN_CIRC:
			t = EaseInCirc(t);
			break;
		case OUT_CIRC:
			t = EaseOutCirc(t);
			break;
		case IN_OUT_CIRC:
			t = EaseInOutCirc(t);
			break;
		case IN_BACK:
			t = EaseInBack(t);
			break;
		case OUT_BACK:
			t = EaseOutBack(t);
			break;
		case IN_OUT_BACK:
			t = EaseInOutBack(t);
			break;
		case IN_ELASTIC:
			t = EaseInElastic(t);
			break;
		case OUT_ELASTIC:
			t = EaseOutElastic(t);
			break;
		case IN_OUT_ELASTIC:
			t = EaseInOutElastic(t);
			break;
		case IN_BOUNCE:
			t = EaseInBounce(t);
			break;
		case OUT_BOUNCE:
			t = EaseOutBounce(t);
			break;
		case IN_OUT_BOUNCE:
			t = EaseInOutBounce(t);
			break;
	}


	if (tween->elapsedTime >= tween->duration)
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

		if (tween->destroyOnComplete)
		{
			RemoveFromList(app->allTweeners, tween);
			DestroyTween(tween);
		}
		else
		{
			RemoveFromList(app->allTweeners, tween);
			tween->isFinished = true;
		}
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


void DestroyTween(Tween *tween)
{
	if (tween == NULL)
	{
		return;
	}

	free(tween);
}


void DestroySequence(TweenSequence *tweenSequence)
{
	if (tweenSequence == NULL)
	{
		return;
	}

	for (int i = 0; i < tweenSequence->tweeners->size; ++i)
	{
		Tween *tween = tweenSequence->tweeners->elements[i];
		DestroyTween(tween);
	}

	free(tweenSequence);
}


float EaseLinear(float t)
{
	return t;
}


float EaseInSine(float t)
{
	return 1 - cos(t * PI / 2);
}


float EaseOutSine(float t)
{
	return sin(t * PI / 2);
}


float EaseInOutSine(float t)
{
	return -(cos(PI * t) - 1) / 2;
}


float EaseInQuad(float t)
{
	return t * t;
}


float EaseOutQuad(float t)
{
	return t * (2 - t);
}


float EaseInOutQuad(float t)
{
	return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}


float EaseInCubic(float t)
{
	return t * t * t;
}


float EaseOutCubic(float t)
{
	return 1 - pow(1 - t, 3);
}


float EaseInOutCubic(float t)
{
	return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}


float EaseInQuart(float t)
{
	return t * t * t * t;
}


float EaseOutQuart(float t)
{
	return 1 - pow(1 - t, 4);
}


float EaseInOutQuart(float t)
{
	return t < 0.5 ? 8 * t * t * t * t : 1 - pow(-2 * t + 2, 4) / 2;
}


float EaseInQuint(float t)
{
	return t * t * t * t * t;
}


float EaseOutQuint(float t)
{
	return 1 - pow(1 - t, 5);
}


float EaseInOutQuint(float t)
{
	return t < 0.5 ? 16 * t * t * t * t * t : 1 - pow(-2 * t + 2, 5) / 2;
}


float EaseInExpo(float t)
{
	return t == 0 ? 0 : pow(2, 10 * (t - 1));
}


float EaseOutExpo(float t)
{
	return t == 1 ? 1 : 1 - pow(2, -10 * t);
}


float EaseInOutExpo(float t)
{
	return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2;
}


float EaseInCirc(float t)
{
	return 1 - sqrt(1 - t * t);
}


float EaseOutCirc(float t)
{
	return sqrt(1 - pow(t - 1, 2));
}


float EaseInOutCirc(float t)
{
	return t < 0.5 ? (1 - sqrt(1 - 4 * t * t)) / 2 : (sqrt(1 - (-2 * t + 2) * (-2 * t + 2)) + 1) / 2;
}


float EaseInBack(float t)
{
	return cbrt(1 - t) * 3 - 2;
}


float EaseOutBack(float t)
{
	return 1 + cbrt(t - 1) * 3;
}


float EaseInOutBack(float t)
{
	return t < 0.5
		       ? (pow(2 * t, 2) * ((2.5 + 1) * 2 * t - 2.5)) / 2
		       : (pow(2 * t - 2, 2) * ((2.5 + 1) * (t * 2 - 2) + 2.5) + 2) / 2;
}


float EaseInElastic(float t)
{
	return sin(13 * PI / 2 * t) * pow(2, 10 * (t - 1));
}


float EaseOutElastic(float t)
{
	return sin(-13 * PI / 2 * (t + 1)) * pow(2, -10 * t) + 1;
}


float EaseInOutElastic(float t)
{
	return t < 0.5
		       ? sin(13 * PI / 2 * 2 * t) * pow(2, 10 * ((2 * t) - 1)) / 2
		       : (sin(-13 * PI / 2 * ((2 * t - 1) + 1)) * pow(2, -10 * (2 * t - 1)) + 2) / 2;
}


float EaseOutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1 / d1)
	{
		return n1 * t * t;
	}
	if (t < 2 / d1)
	{
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	if (t < 2.5 / d1)
	{
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	return n1 * (t -= 2.625f / d1) * t + 0.984375f;
}


float EaseInBounce(float t)
{
	return 1 - EaseOutBounce(1 - t);
}


float EaseInOutBounce(float t)
{
	return t < 0.5 ? (1 - EaseOutBounce(1 - 2 * t)) / 2 : (1 + EaseOutBounce(2 * t - 1)) / 2;
}
