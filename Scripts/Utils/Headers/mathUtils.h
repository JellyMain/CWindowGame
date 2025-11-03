#pragma once
#include "../../General/Headers/structs.h"
#define PI 3.14159265359

float ClampFloat(float value, float min, float max);

int ClampInt(int value, int min, int max);

Vector2Int ClampVector2Int(Vector2Int value, Vector2Int min, Vector2Int max);

Vector2Float ClampVector2Float(Vector2Float value, Vector2Float min, Vector2Float max);

bool IsEntityInBounds(GameEntity *entity, Vector2Float position, Vector2Float boundsMin, Vector2Float boundsMax);

bool IsPointInBounds(Vector2Float point, Vector2Float boundsMin, Vector2Float boundsMax);

bool IsEntityOverlapping(GameEntity *entity1, Vector2Float position1, GameEntity *entity2, Vector2Float position2);

float GetPercentageChange(float oldValue, float newValue);

Vector2Float GetPercentageChangeVector2(Vector2Float oldValue, Vector2Float newValue);

float LerpFloat(float a, float b, float t);

Vector2Float LerpVector2Float(Vector2Float a, Vector2Float b, float t);

void TweenVector2Float(Vector2Float *from, Vector2Float to, float duration);

int LerpInt(int a, int b, float t);

Vector2Int LerpVector2Int(Vector2Int a, Vector2Int b, float t);
