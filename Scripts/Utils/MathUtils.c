#include "MathUtils.h"


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


Vector2Int ClampVector2(Vector2Int value, Vector2Int min, Vector2Int max)
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


bool IsEntityInBounds(Entity *entity, Vector2Int position, Vector2Int boundsMin, Vector2Int boundsMax)
{
	int entityLeft = position.x - entity->size.x / 2;
	int entityTop = position.y - entity->size.y / 2;
	int entityRight = position.x + entity->size.x / 2;
	int entityBottom = position.y + entity->size.y / 2;

	return entityLeft >= boundsMin.x &&
	       entityRight <= boundsMax.x &&
	       entityTop >= boundsMin.y &&
	       entityBottom <= boundsMax.y;
}


int GetPercentageChange(int oldValue, int newValue)
{
	if (oldValue == 0)
	{
		return 0;
	}
	return (newValue - oldValue) * 100 / oldValue;
}

Vector2Int GetPercentageChangeVector2(Vector2Int oldValue, Vector2Int newValue)
{
	Vector2Int result;
	result.x = GetPercentageChange(oldValue.x, newValue.x);
	result.y = GetPercentageChange(oldValue.y, newValue.y);
	return result;
}
