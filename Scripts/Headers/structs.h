#pragma once
#include <SDL.h>
#include "../DataStructures/Headers/List.h"
#include "../DataStructures/Headers/Dictionary.h"


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


typedef enum
{
	WINDOW_SCREEN_SPACE,
	WINDOW_WORLD_SPACE,
} WindowRenderType;


typedef enum
{
	RESIZABLE,
	FIXED_SIZE,
	SCALE_WITH_RESIZE
} WindowType;


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


typedef struct
{
	SDL_Rect characterRects[256];
	SDL_Surface *atlasSurface;
	Dictionary *windowTexturesDictionary;
} TextAtlas;


typedef struct
{
	List *allGameEntities;
	List *allUIEntities;
	List *allGizmosEntities;
	Dictionary *gizmosEntitiesDrawDictionary;
	Dictionary *gameEntitiesDrawDictionary;
	Dictionary *uiEntitiesDrawDictionary;
	TextAtlas *textAtlas;
	bool hasWon;
	bool showGizmos;
} App;


typedef struct
{
	SDL_Window *sdlWindow;
	Vector2Int windowPosition;
	Vector2Int windowSize;
	Vector2Int viewportOffset;
	Vector2Int lastFrameSize;
	SDL_Renderer *renderer;
	WindowRenderType renderType;
	WindowType windowType;
	List *entitiesInWindowList;
} Window;
