#pragma once
#include "../Headers/structs.h"

void TweenFloat(float *from, float to, float duration);

void CreateTween(App *app, TweenType tweenType, TweenData tweenData, float duration, TweenEasingType easingType);

void UpdateTweeners(App *app, float deltaTime);

void DestroyTween(Tween *tween);
