#pragma once
#include <SDL.h>
#include "DataStructures/Headers/list.h"
#include "DataStructures/Headers/dictionary.h"
#include "glad/glad.h"
#include "../CCerealLib/include/serializer.h"
#define VECTOR2_FLOAT_ONE (Vector2Float){1.0f, 1.0f}
#define VECTOR2_INT_ONE (Vector2Int){1, 1}
#define VECTOR2_FLOAT_ZERO (Vector2Float){0.0f, 0.0f}
#define VECTOR2_INT_ZERO (Vector2Int){0,0}


typedef enum
{
	NONE_GAME_STATE,
	GAMEPLAY_GAME_STATE,
	MENU_GAME_STATE,
	GAME_OVER_GAME_STATE,
	LEVEL_EDITOR_GAME_STATE
} GameState;


typedef struct
{
	GLuint textureId;
	int width;
	int height;
} Texture;

typedef SDL_Keycode KeyboardKey;


typedef struct
{
	SDL_Rect characterRects[256];
	Texture *atlasTexture;
} TextAtlas;


typedef struct
{
	float fps;
	int drawCalls;
} DebugData;


typedef struct
{
	List *levelEditorWindows;
} LevelEditorData;


typedef struct
{
	struct LevelData *levelData;
	struct UpdateSystem *updateSystem;
	LevelEditorData *levelEditorData;
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
	bool debugMode;
	SDL_GLContext glContext;
	struct Renderer *renderer;
	SDL_Window *hiddenWindow;
	struct Window *focusedWindow;
	float deltaTime;
	float time;
	DebugData debugData;
} App;


typedef struct
{
	int x;
	int y;
} Vector2Int;


typedef struct SERIALIZABLE
{
	float x;
	float y;
} Vector2Float;


typedef enum
{
	HORIZONTAL_CENTERED,
	HORIZONTAL_NOT_CENTERED,
} DynamicTextHorizontalAlignment;


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
	GIZMO,
	INPUT_FIELD,
	DYNAMIC_TEXT
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
	GLuint entitiesVAO;
	GLuint postProcessingVAO;
	GLuint postProcessingVBO;
	GLuint gizmosVAO;
	GLuint VBO;
	GLuint EBO;
	struct Material *defaultMaterial;
	struct Material *defaultGizmoMaterial;
	struct Material *defaultPostProcessingMaterial;
	Dictionary *postProcessingEffects;
} Renderer;


typedef struct GameEntity
{
	Vector2Float worldPosition;
	Vector2Float scale;
	Vector2Float originalSize;
	Vector2Float size;
	struct Material *material;
	Texture *texture;
} GameEntity;


typedef struct
{
	char *text;
	int maxLength;
	struct Updatable *readKeyboardInputUpdatable;
} InputFieldData;


typedef struct
{
	char *text;
	Vector2Float textScale;
} DynamicTextData;


typedef union
{
	InputFieldData *inputFieldData;
	DynamicTextData *dynamicTextData;
} UIData;


typedef struct UIEntity
{
	Vector2Float worldPosition;
	Vector2Float scale;
	Vector2Float parentScale;
	Vector2Float lastFrameWorldPosition;
	Vector2Float originalSize;
	Vector2Float size;
	struct Material *material;
	Texture *texture;
	UIType uiType;
	UIData *uiData;
	struct UIEntity *parentEntity;
	List *childEntities;
	bool isHovered;
	struct UIInteraction *uiInteraction;
	void (*OnHover)(App *app, struct UIEntity *uiEntity);
	void (*OnHoverExit)(App *app, struct UIEntity *uiEntity);
} UIEntity;


typedef struct UIInteraction
{
	void *data;
	void (*OnInteraction)(App *app, void *data);
	void (*OnInteractionAnimation)(App *app, UIEntity *uiEntity);
} UIInteraction;


typedef struct
{
	UIEntity *connectedEntity;
	SDL_Color color;
	float thickness;
} GizmoEntity;


typedef struct Material
{
	GLuint shaderProgram;
	Dictionary *materialUniforms;
} Material;


typedef struct Window
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
	List *gameEntitiesDrawList;
	List *uiEntitiesDrawList;
	List *gizmosEntitiesDrawList;
	Vector2Float windowCenterPoint;
	Vector2Float upperRightPoint;
	Vector2Float lowerRightPoint;
	Vector2Float upperLeftPoint;
	Vector2Float lowerLeftPoint;
	GLuint FBO;
	GLuint FBOTexture;
	GLuint RBO;
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


typedef struct Updatable
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


typedef enum
{
	UNIFORM_FLOAT,
	UNIFORM_MAT4F
} UniformType;


typedef struct
{
	UniformType uniformType;
	void *uniformValue;
} UniformTypeValuePair;


typedef struct SERIALIZABLE
{
	float randomNumber;
} PlayerData;
