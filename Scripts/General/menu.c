#include "Headers/menu.h"

#include "Infrastructure/Headers/stateMachine.h"
#include "Infrastructure/Headers/update.h"
#include "Headers/structs.h"
#include "UI/Headers/ui.h"
#include "Infrastructure/Headers/window.h"
#include "Render/Headers/draw.h"
#include "Tween/Headers/tweener.h"
#include "Render/Headers/textures.h"


void PlayButtonInteraction(App *app, void *data);
void OnPlayButtonHover(App *app, UIEntity *uiEntity);
void OnPlayButtonHoverExit(App *app, UIEntity *uiEntity);
void LevelEditorButtonInteraction(App *app, void *data);
void UpdatePlayButton(void *data, App *app, float deltaTime);


void CreateMainMenu(App *app)
{
	Window *menuWindow = CreateGameWindowWithRenderer(app, GetDisplayCenterPosition(),
	                                                  (Vector2Int){500, 600}, WINDOW_SCREEN_SPACE,
	                                                  FIXED_SIZE, "Menu");


	SDL_SetWindowPosition(menuWindow->sdlWindow, menuWindow->position.x - menuWindow->size.x / 2,
	                      menuWindow->position.y - menuWindow->size.y / 2);

	Texture *playButtonTexture = LoadTexture("NewButton.png");
	Texture *levelEditorTexture = LoadTexture("NewButton.png");

	Material *playButtonMaterial = CreateMaterial(NULL, NULL);
	Material *levelEditorMaterial = CreateMaterial(NULL, NULL);


	UIInteraction *playButtonInteraction = CreateUIInteraction(NULL, PlayButtonInteraction, NULL);
	UIInteraction *levelEditorInteraction = CreateUIInteraction(
		NULL, LevelEditorButtonInteraction, NULL);

	UIEntity *playButton = CreateButton(menuWindow, playButtonTexture, playButtonMaterial,
	                                    (Vector2Float){100, 50},
	                                    (SDL_Color){0, 0, 0},
	                                    "Play",
	                                    (Vector2Float){0.2f, 0.2f},
	                                    (SDL_Color){0, 0, 0}, app, menuWindow->windowCenterPoint,
	                                    (Vector2Float){2, 2}, playButtonInteraction,
	                                    OnPlayButtonHover,
	                                    OnPlayButtonHoverExit,
	                                    NULL);


	UIEntity *levelEditorButton = CreateButton(menuWindow, levelEditorTexture, levelEditorMaterial,
	                                           (Vector2Float){100, 50},
	                                           (SDL_Color){0, 1, 0}, "Level Editor",
	                                           (Vector2Float){0.2f, 0.2f},
	                                           (SDL_Color){0, 0, 0}, app, (Vector2Float){
		                                           menuWindow->windowCenterPoint.x,
		                                           menuWindow->windowCenterPoint.y + 150
	                                           }, (Vector2Float){2, 2}, levelEditorInteraction,
	                                           OnPlayButtonHover,
	                                           OnPlayButtonHoverExit, NULL);


	Updatable *playButtonUpdatable = CreateUpdatable(playButton, UpdatePlayButton);

	AddUpdatable(app, playButtonUpdatable);
}


void UpdatePlayButton(void *data, App *app, float deltaTime)
{
	UIEntity *playButton = data;
	SetShaderUniform(playButton->material->shaderProgram, "time", UNIFORM_FLOAT, &app->time);
}


void PlayButtonInteraction(App *app, void *data)
{
	SetPendingState(app, GAMEPLAY_GAME_STATE);
}


void LevelEditorButtonInteraction(App *app, void *data)
{
	SetPendingState(app, LEVEL_EDITOR_GAME_STATE);
}


void OnPlayButtonHoverExit(App *app, UIEntity *uiEntity)
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


void OnPlayButtonHover(App *app, UIEntity *uiEntity)
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
