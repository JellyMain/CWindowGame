#pragma once
#include "../Headers/structs.h"

float ClampFloat(float value, float min, float max);

int ClampInt(int value, int min, int max);

Vector2Int ClampVector2(Vector2Int value, Vector2Int min, Vector2Int max);

bool IsEntityInBounds(Entity *entity, Vector2Int position, Vector2Int boundsMin, Vector2Int boundsMax);

int GetPercentageChange(int oldValue, int newValue);

Vector2Int GetPercentageChangeVector2(Vector2Int oldValue, Vector2Int newValue);


