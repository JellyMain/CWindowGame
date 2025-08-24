#pragma once
#include "../DataStructures/Headers/List.h"
#include <SDL.h>

typedef struct
{
	int x;
	int y;
} Vector2;


typedef struct
{
	Vector2 worldPosition;
	Vector2 scale;
	Vector2 size;
	List *texturesList;
} Entity;



