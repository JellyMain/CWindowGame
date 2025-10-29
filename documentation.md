# Documentation

## Tweeners

To create a tween first you need to create a tween data:

```c 
TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1.5f, 1.5f},
			.endValue = {1, 1}
		}
	};
```

Tween data is a union, so you can choose which type of tween you want to create.After that you can specify "from" and "
end" values for that tween.

After that you can create a tween with the following function:

```c 
Tween *CreateTween(TweenType tweenType, void *target, TweenData tweenData, float duration, bool destroyOnComplete, TweenEasingType easingType)
```

Here you need to specify the tween type, target, tween data, duration, destroy on complete and easing type.
For now this types of easing are supported:
Linear
InSine
OutSine
InOutSine
InQuad
OutQuad
InOutQuad
InCubic
OutCubic
InOutCubic
InQuart
OutQuart
InOutQuart
InQuint
OutQuint
InOutQuint
InExpo
OutExpo
InOutExpo
InCirc
OutCirc
InOutCirc
InBack
OutBack
InOutBack
InElastic
OutElastic
InOutElastic
InBounce
OutBounce
InOutBounce

After tween is created you can start it with the following function:

```
void PlayTween(App *app, Tween *tween)
```

Here is a full example of tween creation:

```c
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
```

After you play the tween, it will first check if the current target is already being animated or not in this function:

```c
void SetActiveTweenForTarget(App *app, Tween *tween)
{
	if (DictionaryHasKey(app->tweenTargetsDictionary, tween->target))
	{
		Tween *playingTween = DictionaryGet(app->tweenTargetsDictionary, tween->target);
		if (playingTween != NULL)
		{
			bool foundInSequence = false;

			for (int i = 0; i < app->allTweenSequences->size; i++)
			{
				TweenSequence *tweenSequence = app->allTweenSequences->elements[i];

				for (int j = 0; j < tweenSequence->tweeners->size; j++)
				{
					Tween *sequenceTween = tweenSequence->tweeners->elements[j];
					if (sequenceTween == playingTween)
					{
						foundInSequence = true;
						FinishSequence(app, tweenSequence);
						break;
					}
				}
			}

			if (!foundInSequence)
			{
				FinishTween(app, playingTween);
			}
		}

		DictionaryChangeValue(app->tweenTargetsDictionary, tween->target, tween);
	}
	else
	{
		DictionaryAdd(app->tweenTargetsDictionary, tween->target, tween);
	}
}
```

For that I use a "tweenTargetsDictionary" which has targets that are or were animated as keys and tweeners that animate
those targets as values.
If target is being animated, it will check if this tween is in any sequence and if it is, it will finish the sequence.If
it is not, it will finish the tween.


