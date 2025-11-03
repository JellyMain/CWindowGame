#include "Headers/mathUtils.h"


float ClampFloat(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}

	return value;
}


int ClampInt(int value, int min, int max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}


Vector2Int ClampVector2Int(Vector2Int value, Vector2Int min, Vector2Int max)
{
	Vector2Int result = value;

	if (value.x < min.x)
	{
		result.x = min.x;
	}
	else if (value.x > max.x)
	{
		result.x = max.x;
	}

	if (value.y < min.y)
	{
		result.y = min.y;
	}
	else if (value.y > max.y)
	{
		result.y = max.y;
	}


	return result;
}


Vector2Float ClampVector2Float(Vector2Float value, Vector2Float min, Vector2Float max)
{
	Vector2Float result = value;
	if (value.x < min.x)
	{
		result.x = min.x;
	}
	else if (value.x > max.x)
	{
		result.x = max.x;
	}
	if (value.y < min.y)
	{
		result.y = min.y;
	}
	else if (value.y > max.y)
	{
		result.y = max.y;
	}
	return result;
}


bool IsEntityInBounds(GameEntity *entity, Vector2Float position, Vector2Float boundsMin, Vector2Float boundsMax)
{
	float entityLeft = position.x - entity->size.x / 2;
	float entityTop = position.y - entity->size.y / 2;
	float entityRight = position.x + entity->size.x / 2;
	float entityBottom = position.y + entity->size.y / 2;

	return entityLeft >= boundsMin.x &&
	       entityRight <= boundsMax.x &&
	       entityTop >= boundsMin.y &&
	       entityBottom <= boundsMax.y;
}


bool IsPointInBounds(Vector2Float point, Vector2Float boundsMin, Vector2Float boundsMax)
{
	return point.x >= boundsMin.x && point.x <= boundsMax.x &&
	       point.y >= boundsMin.y && point.y <= boundsMax.y;
}


bool IsEntityOverlapping(GameEntity *entity1, Vector2Float position1, GameEntity *entity2, Vector2Float position2)
{
	float entity1Left = position1.x - entity1->size.x / 2;
	float entity1Right = position1.x + entity1->size.x / 2;
	float entity1Top = position1.y - entity1->size.y / 2;
	float entity1Bottom = position1.y + entity1->size.y / 2;

	float entity2Left = position2.x - entity2->size.x / 2;
	float entity2Right = position2.x + entity2->size.x / 2;
	float entity2Top = position2.y - entity2->size.y / 2;
	float entity2Bottom = position2.y + entity2->size.y / 2;

	return entity1Left < entity2Right &&
	       entity1Right > entity2Left &&
	       entity1Top < entity2Bottom &&
	       entity1Bottom > entity2Top;
}


float GetPercentageChange(float oldValue, float newValue)
{
	if (oldValue == 0)
	{
		return 0;
	}
	return (newValue - oldValue) * 100 / oldValue;
}


Vector2Float GetPercentageChangeVector2(Vector2Float oldValue, Vector2Float newValue)
{
	Vector2Float result;
	result.x = GetPercentageChange(oldValue.x, newValue.x);
	result.y = GetPercentageChange(oldValue.y, newValue.y);
	return result;
}


float LerpFloat(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}


Vector2Float LerpVector2Float(Vector2Float a, Vector2Float b, float t)
{
	Vector2Float result;
	result.x = LerpFloat(a.x, b.x, t);
	result.y = LerpFloat(a.y, b.y, t);
	return result;
}


int LerpInt(int a, int b, float t)
{
	return (int) LerpFloat((float) a, (float) b, t);
}


Vector2Int LerpVector2Int(Vector2Int a, Vector2Int b, float t)
{
	Vector2Int result;
	result.x = LerpInt(a.x, b.x, t);
	result.y = LerpInt(a.y, b.y, t);
	return result;
}
