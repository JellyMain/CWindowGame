#pragma once
#include "../Headers/structs.h"

void TweenFloat(float *from, float to, float duration);

Tween *CreateTween(TweenType tweenType, void *target, TweenData tweenData, float duration,
                   bool destroyOnComplete,
                   TweenEasingType easingType);


TweenSequence *CreateTweenSequence();

void PlayTweenSequence(App *app, TweenSequence *tweenSequence);

void PlayTween(App *app, Tween *tween);

Updatable *CreateTweenersUpdatable();

void AddTweenToSequence(TweenSequence *tweenSequence, Tween *tween);

void DestroyTween(Tween *tween);

void DestroySequence(TweenSequence *tweenSequence);
