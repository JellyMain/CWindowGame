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


typedef enum
{
	TEXT,
	BUTTON,
	IMAGE,
	SLIDER,
	GIZMO
} UIType;


typedef struct
{
	Vector2Int worldPosition;
	Vector2Float scale;
	Vector2Int size;
	List *texturesList;
} GameEntity;


typedef struct UIEntity
{
	Vector2Int worldPosition;
	Vector2Float scale;
	Vector2Int size;
	SDL_Texture *texture;
	UIType uiType;
	struct UIEntity *parentEntity;
	List *childEntities;

	void (*OnInteraction)();
} UIEntity;


typedef struct
{
	UIEntity *connectedEntity;
	SDL_Color color;
	int thickness;
} GizmoEntity;
