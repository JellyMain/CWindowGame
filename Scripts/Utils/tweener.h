#pragma once
#include "../Headers/structs.h"

void TweenFloat(float *from, float to, float duration);

Tween *CreateTween(TweenType tweenType, void *target, TweenData tweenData, float duration, bool destroyOnComplete,
                   TweenEasingType easingType);

void UpdateTweeners(App *app, float deltaTime);

TweenSequence *CreateTweenSequence();

void PlayTweenSequence(App *app, TweenSequence *tweenSequence);

void PlayTween(App *app, Tween *tween);

void AddTweenToSequence(TweenSequence *tweenSequence, Tween *tween);

void DestroyTween(Tween *tween);

void DestroySequence(TweenSequence *tweenSequence);
