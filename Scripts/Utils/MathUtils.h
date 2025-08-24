#pragma once
#include "../Headers/structs.h"

float ClampFloat(float value, float min, float max);

int ClampInt(int value, int min, int max);

Vector2 ClampVector2(Vector2 value, Vector2 min, Vector2 max);

bool IsEntityInBounds(Entity *entity, Vector2 position, Vector2 boundsMin, Vector2 boundsMax);

int GetPercentageChange(int oldValue, int newValue);

Vector2 GetPercentageChangeVector2(Vector2 oldValue, Vector2 newValue);


