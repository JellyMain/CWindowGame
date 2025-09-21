#pragma once
#include <SDL.h>
#include "../DataStructures/Headers/List.h"
#include "../DataStructures/Headers/Dictionary.h"


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
	List *allTweeners;
	Dictionary *gizmosEntitiesDrawDictionary;
	Dictionary *gameEntitiesDrawDictionary;
	Dictionary *uiEntitiesDrawDictionary;
	TextAtlas *textAtlas;
	bool hasWon;
	bool showGizmos;
} App;

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
	VECTOR2_FLOAT_TWEEN,
	FLOAT_TWEEN,
} TweenType;


typedef enum
{
	IN_SINE,
	OUT_SINE,
	IN_OUT_SINE,
	IN_QUAD,
	OUT_QUAD,
	IN_OUT_QUAD,
	IN_CUBIC,
	OUT_CUBIC,
	IN_OUT_CUBIC,
	IN_QUART,
	OUT_QUART,
	IN_OUT_QUART,
	IN_QUINT,
	OUT_QUINT,
	IN_OUT_QUINT,
	IN_EXPO,
	OUT_EXPO,
	IN_OUT_EXPO,
	IN_CIRC,
	OUT_CIRC,
	IN_OUT_CIRC,
	IN_BACK,
	OUT_BACK,
	IN_OUT_BACK,
	IN_ELASTIC,
	OUT_ELASTIC,
	IN_OUT_ELASTIC,
	IN_BOUNCE,
	OUT_BOUNCE,
	IN_OUT_BOUNCE,
	LINEAR,
} TweenEasingType;

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
	Vector2Int originalSize;
	Vector2Int size;
	List *texturesList;
} GameEntity;


typedef struct UIEntity
{
	Vector2Int worldPosition;
	Vector2Float scale;
	Vector2Int originalSize;
	Vector2Int size;
	SDL_Texture *texture;
	UIType uiType;
	struct UIEntity *parentEntity;
	List *childEntities;

	void (*OnInteraction)();

	void (*OnInteractionAnimation)(App *app, struct UIEntity *uiEntity);
} UIEntity;


typedef struct
{
	UIEntity *connectedEntity;
	SDL_Color color;
	int thickness;
} GizmoEntity;


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


typedef union
{
	struct
	{
		Vector2Float *target;
		Vector2Float fromValue;
		Vector2Float endValue;
	} vector2FloatTween;

	struct
	{
		float *target;
		float fromValue;
		float endValue;
	} floatTween;
} TweenData;


typedef struct
{
	TweenType tweenType;
	TweenData tweenData;
	float duration;
	float elapsedTime;
	TweenEasingType easingType;
} Tween;
