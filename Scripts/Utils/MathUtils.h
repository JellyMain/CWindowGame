#pragma once
#include "../Headers/structs.h"

float ClampFloat(float value, float min, float max);

int ClampInt(int value, int min, int max);

Vector2Int ClampVector2Int(Vector2Int value, Vector2Int min, Vector2Int max);

Vector2Float ClampVector2Float(Vector2Float value, Vector2Float min, Vector2Float max);

bool IsEntityInBounds(GameEntity *entity, Vector2Int position, Vector2Int boundsMin, Vector2Int boundsMax);

bool IsPointInBounds(Vector2Int point, Vector2Int boundsMin, Vector2Int boundsMax);

float GetPercentageChange(float oldValue, float newValue);

Vector2Float GetPercentageChangeVector2(Vector2Float oldValue, Vector2Float newValue);


