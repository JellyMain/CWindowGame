#include "ViewPortUtils.h"

Vector2 WorldToScreen(Vector2 worldPosition, Vector2 viewportOffset)
{
	return (Vector2){
		worldPosition.x - viewportOffset.x,
		worldPosition.y - viewportOffset.y
	};
}

