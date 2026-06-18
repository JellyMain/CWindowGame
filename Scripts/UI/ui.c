#include <SDL_image.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include "UI/Headers/ui.h"
#include "Infrastructure/Headers/update.h"
#include "Render/Headers/draw.h"
#include "Input/Headers/input.h"
#include "General/Headers/structs.h"
#include "Infrastructure/Headers/stateMachine.h"
#include "Infrastructure/Headers/window.h"
#include "Utils/Headers/mathUtils.h"
#include "Render/Headers/textures.h"


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness,
                 UIEntity *connectedEntity);

Updatable *CreateReadKeyboardInputUpdatable(UIEntity *inputFieldEntity);

void ReadKeyboardInput(void *data, App *app, float deltaTime);

void StartReadingKeyboardInput(App *app, void *data);


UIInteraction *CreateUIInteraction(void *interactionData,
                                   void (*OnInteraction)(App *app, void *data),
                                   void (*OnInteractionAnimation)(
	                                   App *app, UIEntity *uiEntity))
{
	UIInteraction *uiInteractionData = calloc(1, sizeof(UIInteraction));
	uiInteractionData->data = interactionData;
	uiInteractionData->OnInteraction = OnInteraction;
	uiInteractionData->OnInteractionAnimation = OnInteractionAnimation;
	return uiInteractionData;
}


TextAtlas *CreateTextAtlas(char *fileName, int fontSize)
{
	TextAtlas *textAtlas = calloc(1, sizeof(TextAtlas));
	memset(textAtlas->characterRects, 0, sizeof(textAtlas->characterRects));

	char buffer[150];
	snprintf(buffer, sizeof(buffer), "%s%s", "Assets/", fileName);
	TTF_Font *font = TTF_OpenFont(buffer, fontSize);

	if (font == NULL)
	{
		SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
		free(textAtlas);
		return NULL;
	}

	char *characters =
			"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ";

	int numberOfCharacters = strlen(characters);

	int charWidth;
	int charHeight;
	int charsMaxHeight = 0;

	int atlasWidth = 0;
	int atlasHeight = 0;

	for (int i = 0; i < numberOfCharacters; i++)
	{
		char character = characters[i];
		TTF_SizeText(font, &character, &charWidth, &charHeight);
		if (charHeight > charsMaxHeight)
		{
			charsMaxHeight = charHeight;
		}

		SDL_Rect characterRect = {atlasWidth, 0, charWidth, charHeight};
		textAtlas->characterRects[(int) character] = characterRect;

		atlasWidth += charWidth;
	}

	atlasHeight = charsMaxHeight;

	SDL_Surface *atlasSurface = SDL_CreateRGBSurface(0, atlasWidth, atlasHeight, 32,
	                                                 0x000000FF,
	                                                 0x0000FF00,
	                                                 0x00FF0000,
	                                                 0xFF000000);

	if (!atlasSurface)
	{
		SDL_Log("Unable to create texture surface: %s", SDL_GetError());
		TTF_CloseFont(font);
		free(textAtlas);
		return NULL;
	}

	// Fill with transparent color (RGBA: 0, 0, 0, 0)
	SDL_FillRect(atlasSurface, NULL, SDL_MapRGBA(atlasSurface->format, 0, 0, 0, 0));

	for (int i = 0; i < numberOfCharacters; i++)
	{
		SDL_Rect characterRect = textAtlas->characterRects[(int) characters[i]];
		char text[2] = {characters[i], '\0'};

		SDL_Surface *characterSurface = TTF_RenderText_Blended(
			font, text, (SDL_Color){255, 255, 255, 255});

		if (characterSurface == NULL)
		{
			continue;
		}

		SDL_BlitSurface(characterSurface, NULL, atlasSurface, &characterRect);

		textAtlas->characterRects[(int) characters[i]] = characterRect;

		SDL_FreeSurface(characterSurface);
	}

	textAtlas->atlasTexture = CreateTextureFromSurface(atlasSurface);

	SDL_FreeSurface(atlasSurface);
	TTF_CloseFont(font);

	return textAtlas;
}


UIEntity *CreateStaticText(Window *window, App *app, char *text, SDL_Color textColor,
                           Vector2Float position,
                           Vector2Float scale,
                           UIEntity *parent)
{
	UIEntity *textEntity = calloc(1, sizeof(UIEntity));

	textEntity->scale = scale;
	textEntity->parentEntity = parent;
	textEntity->childEntities = ListCreate(0);
	textEntity->worldPosition = position;
	textEntity->lastFrameWorldPosition = position;
	textEntity->isHovered = false;

	if (parent != NULL)
	{
		textEntity->parentScale.x = parent->parentScale.x * parent->scale.x;
		textEntity->parentScale.y = parent->parentScale.y * parent->scale.y;
	}
	else
	{
		textEntity->parentScale = VECTOR2_FLOAT_ONE;
	}


	char buffer[150];
	snprintf(buffer, sizeof(buffer), "%s%s", "Assets/", "Sillyfont-Regular.ttf");
	TTF_Font *font = TTF_OpenFont(buffer, 64);

	if (!font)
	{
		SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
		return NULL;
	}

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);

	if (!textSurface)
	{
		SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
		TTF_CloseFont(font);
		return NULL;
	}

	Texture *texture = CreateTextureFromSurface(textSurface);
	textEntity->texture = texture;
	textEntity->material = CreateMaterial(NULL, NULL);

	textEntity->uiType = TEXT;

	textEntity->originalSize.x = textEntity->texture->width;
	textEntity->originalSize.y = textEntity->texture->height;

	TTF_CloseFont(font);
	SDL_FreeSurface(textSurface);

	ListAdd(app->allUIEntities, textEntity);
	AddUIEntityToDrawList(window, textEntity);

	return textEntity;
}


UIEntity *CreateButton(Window *window, Texture *buttonTexture, Material *buttonMaterial,
                       Vector2Float size,
                       SDL_Color backgroundColor,
                       char *text, Vector2Float textScale,
                       SDL_Color textColor, App *app, Vector2Float position,
                       Vector2Float buttonScale, UIInteraction *uiInteraction,
                       void (*OnHover)(App *app, UIEntity *uiEntity),
                       void (*OnHoverExit)(App *app, UIEntity *uiEntity),
                       UIEntity *parent)
{
	UIEntity *buttonEntity = calloc(1, sizeof(UIEntity));
	buttonEntity->scale = buttonScale;
	buttonEntity->uiType = BUTTON;
	buttonEntity->isHovered = false;
	buttonEntity->childEntities = ListCreate(0);
	buttonEntity->parentEntity = parent;
	buttonEntity->uiInteraction = uiInteraction;
	buttonEntity->OnHover = OnHover;
	buttonEntity->OnHoverExit = OnHoverExit;
	buttonEntity->material = buttonMaterial;


	if (parent != NULL)
	{
		buttonEntity->parentScale.x = parent->parentScale.x * parent->scale.x;
		buttonEntity->parentScale.y = parent->parentScale.y * parent->scale.y;
	}
	else
	{
		buttonEntity->parentScale = VECTOR2_FLOAT_ONE;
	}


	buttonEntity->worldPosition = position;
	buttonEntity->lastFrameWorldPosition = position;


	if (buttonTexture == NULL)
	{
		Texture *rectTexture = CreateRect(size, backgroundColor);
		buttonEntity->texture = rectTexture;
	}
	else
	{
		buttonEntity->texture = buttonTexture;
	}

	if (buttonMaterial == NULL)
	{
		buttonEntity->material = CreateMaterial(NULL, NULL);
	}

	buttonEntity->originalSize.x = buttonEntity->texture->width;
	buttonEntity->originalSize.y = buttonEntity->texture->height;

	ListAdd(app->allUIEntities, buttonEntity);
	AddUIEntityToDrawList(window, buttonEntity);


	if (text != NULL && strlen(text) > 0)
	{
		UIEntity *textEntity = CreateStaticText(window, app, text, textColor, position, textScale,
		                                        buttonEntity);
		ListAdd(buttonEntity->childEntities, textEntity);
	}

	CreateGizmo(app, window, (SDL_Color){255, 0, 0, 255}, 1, buttonEntity);

	return buttonEntity;
}


DynamicTextData *CreateDynamicTextData(char *text)
{
	DynamicTextData *dynamicTextData = calloc(1, sizeof(DynamicTextData));
	dynamicTextData->text = text;
	return dynamicTextData;
}


UIEntity *CreateDynamicText(Window *window, App *app, char *text, Vector2Float position,
                            Vector2Float textScale,
                            UIEntity *parent)
{
	UIEntity *dynamicTextEntity = calloc(1, sizeof(UIEntity));

	dynamicTextEntity->scale = textScale;
	dynamicTextEntity->parentEntity = parent;
	dynamicTextEntity->childEntities = ListCreate(0);
	dynamicTextEntity->worldPosition = position;
	dynamicTextEntity->lastFrameWorldPosition = position;
	dynamicTextEntity->isHovered = false;
	dynamicTextEntity->uiType = DYNAMIC_TEXT;
	dynamicTextEntity->material = CreateMaterial(NULL, NULL);

	if (parent != NULL)
	{
		dynamicTextEntity->parentScale.x = parent->parentScale.x * parent->scale.x;
		dynamicTextEntity->parentScale.y = parent->parentScale.y * parent->scale.y;
	}
	else
	{
		dynamicTextEntity->parentScale = VECTOR2_FLOAT_ONE;
	}

	printf("Parent scale is (%f, %f)\n", dynamicTextEntity->parentScale.x,
	       dynamicTextEntity->parentScale.y);


	dynamicTextEntity->uiData = calloc(1, sizeof(UIData));
	dynamicTextEntity->uiData->dynamicTextData = CreateDynamicTextData(text);

	dynamicTextEntity->texture = NULL;
	dynamicTextEntity->originalSize = VECTOR2_FLOAT_ZERO;

	ListAdd(app->allUIEntities, dynamicTextEntity);
	AddUIEntityToDrawList(window, dynamicTextEntity);

	return dynamicTextEntity;
}


InputFieldData *CreateInputFieldData(UIEntity *inputFieldEntity, int maxLength)
{
	InputFieldData *inputFieldData = calloc(1, sizeof(InputFieldData));
	inputFieldData->text = malloc(sizeof(char) * maxLength + 1);
	inputFieldData->text[0] = '\0';
	inputFieldData->maxLength = maxLength;
	inputFieldData->readKeyboardInputUpdatable = CreateReadKeyboardInputUpdatable(inputFieldEntity);
	return inputFieldData;
}


UIEntity *CreateInputField(Window *window, Texture *inputFieldTexture,
                           Material *inputFieldMaterial,
                           Vector2Float size,
                           SDL_Color backgroundColor, App *app, Vector2Float position,
                           Vector2Float inputFieldScale, Vector2Float textScale, UIEntity *parent)
{
	UIEntity *inputFieldEntity = calloc(1, sizeof(UIEntity));
	inputFieldEntity->scale = inputFieldScale;
	inputFieldEntity->uiType = INPUT_FIELD;
	inputFieldEntity->isHovered = false;
	inputFieldEntity->childEntities = ListCreate(0);
	inputFieldEntity->parentEntity = parent;
	inputFieldEntity->material = inputFieldMaterial;

	inputFieldEntity->uiData = calloc(1, sizeof(UIData));
	inputFieldEntity->uiData->inputFieldData = CreateInputFieldData(inputFieldEntity, 10);

	if (parent != NULL)
	{
		inputFieldEntity->parentScale = parent->parentScale;
	}
	else
	{
		inputFieldEntity->parentScale = VECTOR2_FLOAT_ONE;
	}

	if (inputFieldMaterial == NULL)
	{
		inputFieldEntity->material = CreateMaterial(NULL, NULL);
	}


	inputFieldEntity->worldPosition = position;
	inputFieldEntity->lastFrameWorldPosition = position;


	if (inputFieldTexture == NULL)
	{
		Texture *rectTexture = CreateRect(size, backgroundColor);
		inputFieldEntity->texture = rectTexture;
	}
	else
	{
		inputFieldEntity->texture = inputFieldTexture;
	}

	inputFieldEntity->originalSize.x = inputFieldEntity->texture->width;
	inputFieldEntity->originalSize.y = inputFieldEntity->texture->height;

	inputFieldEntity->uiInteraction = CreateUIInteraction(inputFieldEntity,
	                                                      StartReadingKeyboardInput, NULL);


	ListAdd(app->allUIEntities, inputFieldEntity);
	AddUIEntityToDrawList(window, inputFieldEntity);


	CreateGizmo(app, window, (SDL_Color){255, 0, 0, 255}, 1, inputFieldEntity);


	UIEntity *createdDynamicText = CreateDynamicText(window, app,
	                                                 inputFieldEntity->uiData->inputFieldData->text,
	                                                 inputFieldEntity->worldPosition, textScale,
	                                                 inputFieldEntity);

	ListAdd(inputFieldEntity->childEntities, createdDynamicText);

	return inputFieldEntity;
}


void StartReadingKeyboardInput(App *app, void *data)
{
	UIEntity *inputFieldEntity = data;
	if (!HasUpdatable(app, inputFieldEntity->uiData->inputFieldData->readKeyboardInputUpdatable))
	{
		printf("Starting reading keyboard input\n");
		AddUpdatable(app, inputFieldEntity->uiData->inputFieldData->readKeyboardInputUpdatable);
	}
}


Updatable *CreateReadKeyboardInputUpdatable(UIEntity *inputFieldEntity)
{
	Updatable *readKeyboardInputUpdatable = CreateUpdatable(inputFieldEntity, ReadKeyboardInput);
	return readKeyboardInputUpdatable;
}


void ReadKeyboardInput(void *data, App *app, float deltaTime)
{
	UIEntity *inputFieldEntity = data;

	InputFieldData *inputFieldData = inputFieldEntity->uiData->inputFieldData;

	KeyboardKey key = GetKeyPressed();

	if (key != SDLK_UNKNOWN)
	{
		if (key == SDLK_ESCAPE)
		{
			if (HasUpdatable(
				app, inputFieldEntity->uiData->inputFieldData->readKeyboardInputUpdatable))
			{
				RemoveUpdatable(
					app, inputFieldEntity->uiData->inputFieldData->readKeyboardInputUpdatable);
				printf("removed updatable \n");
			}

			return;
		}

		if (key == SDLK_BACKSPACE)
		{
			int currentLength = strlen(inputFieldData->text);
			if (currentLength > 0)
			{
				inputFieldData->text[currentLength - 1] = '\0';
			}
			return;
		}

		int currentLength = strlen(inputFieldData->text);
		if (currentLength < inputFieldData->maxLength)
		{
			inputFieldData->text[currentLength] = key;
			inputFieldData->text[currentLength + 1] = '\0';
		}
	}
}


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness,
                 UIEntity *connectedEntity)
{
	GizmoEntity *gizmoEntity = calloc(1, sizeof(GizmoEntity));
	gizmoEntity->connectedEntity = connectedEntity;
	gizmoEntity->color = color;
	gizmoEntity->thickness = thickness;
	ListAdd(app->allGizmosEntities, gizmoEntity);
	ListAdd(window->gizmosEntitiesDrawList, gizmoEntity);
}


void UpdateChildrenScale(UIEntity *uiEntity)
{
	if (uiEntity->childEntities->size > 0)
	{
		// Calculate the accumulated scale for this entity
		Vector2Float thisAccumulatedScale = uiEntity->parentScale;
		thisAccumulatedScale.x *= uiEntity->scale.x;
		thisAccumulatedScale.y *= uiEntity->scale.y;

		for (int j = 0; j < uiEntity->childEntities->size; j++)
		{
			UIEntity *child = uiEntity->childEntities->elements[j];
			// Children's parentScale is the accumulated scale of their parent
			child->parentScale = thisAccumulatedScale;
		}
	}
}


void UpdateChildrenPosition(UIEntity *uiEntity)
{
	if (uiEntity->childEntities->size > 0)
	{
		Vector2Int parentPositionDelta = (Vector2Int){
			uiEntity->worldPosition.x - uiEntity->lastFrameWorldPosition.x,
			uiEntity->worldPosition.y - uiEntity->lastFrameWorldPosition.y
		};

		if (parentPositionDelta.x != 0 || parentPositionDelta.y != 0)
		{
			for (int j = 0; j < uiEntity->childEntities->size; j++)
			{
				UIEntity *child = uiEntity->childEntities->elements[j];
				child->worldPosition = (Vector2Float){
					child->worldPosition.x + parentPositionDelta.x,
					child->worldPosition.y + parentPositionDelta.y
				};
			}
		}

		uiEntity->lastFrameWorldPosition = uiEntity->worldPosition;
	}
}


void HandleInteractions(App *app)
{
	if (app->focusedWindow == NULL)
	{
		return;
	}

	for (int i = 0; i < app->focusedWindow->uiEntitiesDrawList->size; i++)
	{
		UIEntity *uiEntity = ListGet(app->focusedWindow->uiEntitiesDrawList, i);

		switch (uiEntity->uiType)
		{
			case INPUT_FIELD:
			case BUTTON:
			{
				Vector2Float mousePosition = GetMousePosition();

				Vector2Float boundsMin = {
					uiEntity->worldPosition.x - uiEntity->size.x / 2,
					uiEntity->worldPosition.y - uiEntity->size.y / 2
				};
				Vector2Float boundsMax = {
					uiEntity->worldPosition.x + uiEntity->size.x / 2,
					uiEntity->worldPosition.y + uiEntity->size.y / 2
				};

				if (IsPointInBounds(mousePosition, boundsMin, boundsMax))
				{
					if (!uiEntity->isHovered)
					{
						uiEntity->isHovered = true;

						if (uiEntity->OnHover != NULL)
						{
							uiEntity->OnHover(app, uiEntity);
						}
					}


					if (IsLeftMouseButtonClicked())
					{
						if (uiEntity->uiInteraction->OnInteraction != NULL)
						{
							uiEntity->uiInteraction->OnInteraction(
								app, uiEntity->uiInteraction->data);
						}
						if (uiEntity->uiInteraction->OnInteractionAnimation != NULL)
						{
							uiEntity->uiInteraction->OnInteractionAnimation(app, uiEntity);
						}
					}
				}
				else
				{
					if (uiEntity->isHovered && uiEntity->OnHoverExit != NULL)
					{
						uiEntity->OnHoverExit(app, uiEntity);
					}
					uiEntity->isHovered = false;
				}
			}
			break;

			default:
				break;
		}
	}
}


void UpdateUIElements(void *data, App *app, float deltaTime)
{
	for (int i = 0; i < app->allUIEntities->size; i++)
	{
		UIEntity *uiEntity = app->allUIEntities->elements[i];

		UpdateChildrenScale(uiEntity);
		UpdateChildrenPosition(uiEntity);
	}

	HandleInteractions(app);
}


Updatable *CreateUIUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateUIElements);
	return updatable;
}
