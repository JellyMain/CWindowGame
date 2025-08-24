#include "ViewPortUtils.h"

Vector2Int WorldToScreen(Vector2Int worldPosition, Vector2Int viewportOffset)
{
	return (Vector2Int){
		worldPosition.x - viewportOffset.x,
		worldPosition.y - viewportOffset.y
	};
}

