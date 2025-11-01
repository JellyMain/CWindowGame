﻿#pragma once
#include <SDL.h>
#include "../../DataStructures/Headers/list.h"
#include "../../DataStructures/Headers/dictionary.h"
#include "glad/glad.h"
#define VECTOR2_FLOAT_ONE (Vector2Float){1.0f, 1.0f}
#define VECTOR2_INT_ONE (Vector2Int){1, 1}
#define VECTOR2_FLOAT_ZERO (Vector2Float){0.0f, 0.0f}
#define VECTOR2_INT_ZERO (Vector2Int){0,0}


typedef enum
{
	NONE_GAME_STATE,
	GAMEPLAY_GAME_STATE,
	MENU_GAME_STATE,
	GAME_OVER_GAME_STATE
} GameState;


typedef struct
{
	GLuint textureId;
	int width;
	int height;
} Texture;


typedef struct
{
	SDL_Rect characterRects[256];
	Texture *atlasTexture;
	Dictionary *windowTexturesDictionary;
} TextAtlas;


typedef struct
{
	struct LevelData *levelData;
	struct UpdateSystem *updateSystem;
	GameState pendingGameState;
	GameState gameState;
	int pixelsPerUnit;
	List *allGameEntities;
	List *allUIEntities;
	List *allGizmosEntities;
	List *allTweeners;
	List *allTweenSequences;
	List *allWindows;
	Dictionary *tweenTargetsDictionary;
	TextAtlas *textAtlas;
	bool hasWon;
	bool showGizmos;
	SDL_GLContext glContext;
	struct Renderer *renderer;
	SDL_Window *hiddenWindow;
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
	VECTOR2_INT_TWEEN,
	INT_TWEEN,
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


typedef struct Renderer
{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint shaderProgram;
	GLuint projectionLocation;
} Renderer;


typedef struct GameEntity
{
	Vector2Int worldPosition;
	Vector2Float scale;
	Vector2Int originalSize;
	Vector2Int size;
	struct Material *material;
} GameEntity;


typedef struct UIEntity
{
	Vector2Int worldPosition;
	Vector2Float entityScale;
	Vector2Float parentScale;
	Vector2Float lastFrameParentScale;
	Vector2Int lastFrameWorldPosition;
	Vector2Int originalSize;
	Vector2Int size;
	struct Material *material;
	UIType uiType;
	struct UIEntity *parentEntity;
	List *childEntities;
	bool isHovered;
	void *interactionData;
	void (*OnHover)(App *app, struct UIEntity *uiEntity);
	void (*OnInteraction)(App *app, void *interactionData);
	void (*OnInteractionAnimation)(App *app, struct UIEntity *uiEntity);
	void (*OnHoverExit)(App *app, struct UIEntity *uiEntity);
} UIEntity;


typedef struct
{
	UIEntity *connectedEntity;
	SDL_Color color;
	int thickness;
} GizmoEntity;


typedef struct Material
{
	Texture *texture;
	GLuint shaderProgram;
	GLuint projectionLocation;
} Material;


typedef struct
{
	SDL_Window *sdlWindow;
	Vector2Int position;
	Vector2Int size;
	Vector2Int viewportOffset;
	Vector2Int lastFrameSize;
	SDL_Renderer *renderer;
	WindowRenderType renderType;
	WindowType windowType;
	List *entitiesInWindowList;
	Vector2Int windowCenterPoint;
	Vector2Int upperRightPoint;
	Vector2Int lowerRightPoint;
	Vector2Int upperLeftPoint;
	Vector2Int lowerLeftPoint;
} Window;


typedef union
{
	struct
	{
		Vector2Float fromValue;
		Vector2Float endValue;
	} vector2FloatTween;

	struct
	{
		float fromValue;
		float endValue;
	} floatTween;

	struct
	{
		Vector2Int fromValue;
		Vector2Int endValue;
	} vector2IntTween;

	struct
	{
		int fromValue;
		int endValue;
	} intTween;
} TweenData;


typedef struct
{
	TweenType tweenType;
	TweenData tweenData;
	float duration;
	float elapsedTime;
	bool isStarted;
	bool isFinished;
	bool destroyOnComplete;
	TweenEasingType easingType;
	void *target;
	int id;
} Tween;


typedef struct
{
	List *tweeners;
	bool isStarted;
	bool isFinished;
} TweenSequence;


typedef struct
{
	void *data;
	void (*Update)(void *self, App *app, float deltaTime);
} Updatable;


typedef struct UpdateSystem
{
	List *updatables;
} UpdateSystem;


typedef struct LevelData
{
	GameEntity *player;
	GameEntity *levelTarget;
} LevelData;
