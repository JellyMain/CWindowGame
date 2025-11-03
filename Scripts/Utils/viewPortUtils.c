#include "../Utils/Headers/viewPortUtils.h"


Vector2Float WorldToScreen(Vector2Float worldPosition, Vector2Int viewportOffset)
{
	return (Vector2Float)
	{
		worldPosition.x - viewportOffset.x,
		worldPosition.y - viewportOffset.y
	};
}
