#pragma once
#include "../DataStructures/Headers/List.h"
#include <SDL.h>

typedef struct
{
	int x;
	int y;
} Vector2Int;


typedef struct
{
	float x;
	float y;
} Vector2Float;


typedef struct
{
	Vector2Int worldPosition;
	Vector2Int scale;
	Vector2Int size;
	List *texturesList;
} Entity;
