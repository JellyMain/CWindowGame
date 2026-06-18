#include "General/Headers/structs.h"
#include "Infrastructure/Headers/window.h"
#include "General/Headers/levelEditor.h"

#include "Infrastructure/Headers/stateMachine.h"
#include "Infrastructure/Headers/update.h"
#include "Render/Headers/textures.h"
#include "Tween/Headers/tweener.h"
#include "UI/Headers/ui.h"


void OnExitButtonHover(App *app, UIEntity *uiEntity);
void OnExitButtonHoverExit(App *app, UIEntity *uiEntity);
void ExitButtonInteraction(App *app, void *data);
void CreateWindowButtonInteraction(App *app, void *data);


LevelEditorData *CreateLevelEditorData()
{
	LevelEditorData *levelEditorData = calloc(1, sizeof(LevelEditorData));
	levelEditorData->levelEditorWindows = ListCreate(0);
	return levelEditorData;
}


void CreateLevelEditor(App *app)
{
	Window *levelEditorWindow = CreateGameWindowWithRenderer(
		app, GetDisplayCenterPosition(), (Vector2Int){800, 800},
		WINDOW_SCREEN_SPACE, FIXED_SIZE, "Level Editor");

	SDL_SetWindowPosition(levelEditorWindow->sdlWindow,
	                      levelEditorWindow->position.x - levelEditorWindow->size.x / 2,
	                      levelEditorWindow->position.y - levelEditorWindow->size.y / 2);


	Texture *windowSizeInputFieldTexture = LoadTexture("NewButton.png");
	Texture *exitButtonTexture = LoadTexture("NewButton.png");
	Texture *createWindowButtonTexture = LoadTexture("NewButton.png");
	Material *windowSizeInputFieldMaterial = CreateMaterial(NULL, NULL);
	Material *exitButtonMaterial = CreateMaterial(NULL, NULL);
	Material *createWindowButtonMaterial = CreateMaterial(NULL, NULL);

	UIInteraction *exitButtonInteraction = CreateUIInteraction(NULL, ExitButtonInteraction, NULL);

	UIEntity *exitToMenuButton = CreateButton(levelEditorWindow, exitButtonTexture,
	                                          exitButtonMaterial,
	                                          (Vector2Float){100, 50},
	                                          (SDL_Color){0, 0, 0}, "Exit",
	                                          (Vector2Float){0.2f, 0.2f},
	                                          (SDL_Color){0, 0, 0}, app, (Vector2Float){
		                                          levelEditorWindow->windowCenterPoint.x,
		                                          levelEditorWindow->windowCenterPoint.y + 150
	                                          }, (Vector2Float){2, 2}, exitButtonInteraction,
	                                          OnExitButtonHover,
	                                          OnExitButtonHoverExit,
	                                          NULL);


	UIEntity *windowSizeInputField = CreateInputField(levelEditorWindow,
	                                                  windowSizeInputFieldTexture,
	                                                  windowSizeInputFieldMaterial,
	                                                  (Vector2Float){100, 50}, (SDL_Color){0, 0, 0},
	                                                  app, (Vector2Float){
		                                                  levelEditorWindow->windowCenterPoint.x,
		                                                  levelEditorWindow->windowCenterPoint.y +
		                                                  300
	                                                  }, (Vector2Float){2, 2}, (Vector2Float){1, 1},
	                                                  NULL);


	// UIEntity *createWindowButton = CreateButton(levelEditorWindow, createWindowButtonTexture,
	//                                             createWindowButtonMaterial, (Vector2Float){100, 50},
	//                                             (SDL_Color){0, 0, 0}, "Create Window",
	//                                             (Vector2Float){0.3f, 0.3f}, (SDL_Color){0, 0, 0},
	//                                             app, (Vector2Float){
	// 	                                            levelEditorWindow->windowCenterPoint.x,
	// 	                                            levelEditorWindow->windowCenterPoint.y + 250
	//                                             }, (Vector2Float){2, 2}, NULL,
	//                                             CreateWindowButtonInteraction, NULL, NULL, NULL,
	//                                             NULL);
}


void OnExitButtonHover(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1, 1},
			.endValue = {1.5f, 1.5f}
		}
	};


	Tween *playButtonTween = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale, tweenData,
	                                     0.5f, true, OUT_QUINT);
	PlayTween(app, playButtonTween);
}


void OnExitButtonHoverExit(App *app, UIEntity *uiEntity)
{
	TweenData tweenData = {
		.vector2FloatTween = {
			.fromValue = {1.5f, 1.5f},
			.endValue = {1, 1}
		}
	};


	Tween *playButtonTween = CreateTween(VECTOR2_FLOAT_TWEEN, &uiEntity->parentScale, tweenData,
	                                     0.5f, true, OUT_QUINT);
	PlayTween(app, playButtonTween);
}


void ExitButtonInteraction(App *app, void *data)
{
	SetPendingState(app, MENU_GAME_STATE);
}


void CreateWindowButtonInteraction(App *app, void *data)
{
	Window *window = CreateGameWindowWithRenderer(app, GetDisplayCenterPosition(),
	                                              (Vector2Int){100, 100}, WINDOW_WORLD_SPACE,
	                                              FIXED_SIZE, "Window1");

	ListAdd(app->levelEditorData->levelEditorWindows, window);
}
