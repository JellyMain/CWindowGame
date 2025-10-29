#include "Headers/ui.h"

#include <SDL_image.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#include "Headers/update.h"
#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/structs.h"
#include "Headers/window.h"
#include "Utils/mathUtils.h"


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness,
                 UIEntity *connectedEntity);


void DrawDynamicText(Window *window, TextAtlas *textAtlas, char *text, Vector2Int position,
                     Vector2Float scale)
{
	int currentX = 0;
	SDL_Texture *atlasTexture = DictionaryGet(textAtlas->windowTexturesDictionary, window);

	for (int i = 0; text[i] != '\0'; i++)
	{
		unsigned char character = (unsigned char) text[i];

		if (textAtlas->characterRects[character].w > 0 && textAtlas->characterRects[character].h >
		    0)
		{
			SDL_Rect charRect = textAtlas->characterRects[character];
			SDL_Rect dstRect = {
				position.x + currentX, position.y, charRect.w * scale.x, charRect.h * scale.y
			};

			SDL_RenderCopy(window->renderer, atlasTexture, &charRect, &dstRect);
			currentX += charRect.w * scale.x;
		}
		else
		{
			currentX += 1;
		}
	}
}


void DrawThickRectBorder(Window *window, Vector2Int position, Vector2Int size, int thickness)
{
	SDL_Rect topRect = {position.x - size.x / 2, position.y - size.y / 2, size.x, thickness};
	SDL_RenderFillRect(window->renderer, &topRect);

	SDL_Rect bottomRect = {
		position.x - size.x / 2, position.y + size.y / 2, size.x, thickness
	};
	SDL_RenderFillRect(window->renderer, &bottomRect);

	SDL_Rect leftRect = {
		position.x - size.x / 2, position.y - size.y / 2 + thickness, thickness,
		size.y - 2 * thickness + 1
	};
	SDL_RenderFillRect(window->renderer, &leftRect);

	SDL_Rect rightRect = {
		position.x + size.x / 2 - thickness, position.y - size.y / 2 + thickness, thickness,
		size.y - 2 * thickness + 1
	};
	SDL_RenderFillRect(window->renderer, &rightRect);
}


TextAtlas *CreateTextAtlas(char *fileName, int fontSize)
{
	TextAtlas *textAtlas = calloc(1, sizeof(TextAtlas));
	memset(textAtlas->characterRects, 0, sizeof(textAtlas->characterRects));
	textAtlas->windowTexturesDictionary = DictionaryCreate(HashWindow, WindowEquals);

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
			"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

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

	SDL_Surface *atlasSurface = SDL_CreateRGBSurface(0, atlasWidth, atlasHeight, 32, 0, 0, 0, 0);

	if (!atlasSurface)
	{
		SDL_Log("Unable to create texture surface: %s", SDL_GetError());
		TTF_CloseFont(font);
		free(textAtlas);
		return NULL;
	}


	SDL_FillRect(atlasSurface, NULL, SDL_MapRGB(atlasSurface->format, 0, 0, 0));

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


	textAtlas->atlasSurface = atlasSurface;

	TTF_CloseFont(font);

	return textAtlas;
}


void AddAtlasTexture(TextAtlas *textAtlas, Window *window)
{
	SDL_Texture *atlasTexture = SDL_CreateTextureFromSurface(
		window->renderer, textAtlas->atlasSurface);

	SDL_SetTextureScaleMode(atlasTexture, SDL_ScaleModeNearest);

	DictionaryAdd(textAtlas->windowTexturesDictionary, window, atlasTexture);
}


UIEntity *CreateStaticText(char *text, SDL_Color textColor, App *app, Window *window, Vector2Int position, Vector2Float scale,
                           UIEntity *parent)
{
	UIEntity *textEntity = calloc(1, sizeof(UIEntity));

	textEntity->entityScale = scale;
	textEntity->parentEntity = parent;
	textEntity->childEntities = ListCreate(0);
	textEntity->worldPosition = position;
	textEntity->lastFrameWorldPosition = position;
	textEntity->isHovered = false;

	if (parent != NULL)
	{
		textEntity->parentScale = parent->parentScale;
	}
	else
	{
		textEntity->parentScale = VECTOR2_FLOAT_ONE;
	}

	textEntity->lastFrameParentScale = textEntity->parentScale;


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

	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(window->renderer, textSurface);

	textEntity->texture = textTexture;

	if (!textTexture)
	{
		SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
		SDL_FreeSurface(textSurface);
		TTF_CloseFont(font);
		return NULL;
	}

	int textWidth = textSurface->w;
	int textHeight = textSurface->h;

	textEntity->uiType = TEXT;
	textEntity->originalSize = (Vector2Int){textWidth, textHeight};

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	AddToUIEntitiesDrawList(app, window, textEntity);

	return textEntity;
}


void CreateButton(SDL_Texture *backgroundTexture, Vector2Int size, SDL_Color backgroundColor,
                  char *text, Vector2Float textScale,
                  SDL_Color textColor, App *app, Window *window, Vector2Int position,
                  Vector2Float buttonScale, void *interactionData,
                  void (*OnInteraction)(App *app, void *data),
                  void (*OnInteractionAnimation)(App *app, UIEntity *uiEntity),
                  void (*OnHover)(App *app, UIEntity *uiEntity),
                  void (*OnHoverExit)(App *app, UIEntity *uiEntity),
                  UIEntity *parent)
{
	UIEntity *buttonEntity = calloc(1, sizeof(UIEntity));
	buttonEntity->entityScale = buttonScale;
	buttonEntity->uiType = BUTTON;
	buttonEntity->isHovered = false;
	buttonEntity->childEntities = ListCreate(0);
	buttonEntity->parentEntity = parent;
	buttonEntity->OnInteraction = OnInteraction;
	buttonEntity->OnInteractionAnimation = OnInteractionAnimation;
	buttonEntity->OnHover = OnHover;
	buttonEntity->OnHoverExit = OnHoverExit;
	buttonEntity->interactionData = interactionData;

	if (parent != NULL)
	{
		buttonEntity->parentScale = parent->parentScale;
	}
	else
	{
		buttonEntity->parentScale = VECTOR2_FLOAT_ONE;
	}

	buttonEntity->lastFrameParentScale = buttonEntity->parentScale;


	buttonEntity->worldPosition = position;
	buttonEntity->lastFrameWorldPosition = position;

	if (backgroundTexture == NULL)
	{
		SDL_Surface *buttonSurface = SDL_CreateRGBSurface(0, size.x, buttonScale.y, 32, 0, 0, 0, 0);
		if (!buttonSurface)
		{
			SDL_Log("Unable to create rectangle surface: %s", SDL_GetError());
			free(buttonEntity);
			return;
		}

		SDL_FillRect(buttonSurface, NULL,
		             SDL_MapRGB(buttonSurface->format, backgroundColor.r, backgroundColor.g,
		                        backgroundColor.b));

		buttonEntity->texture = SDL_CreateTextureFromSurface(window->renderer, buttonSurface);
		buttonEntity->originalSize = size;

		SDL_FreeSurface(buttonSurface);

		if (!buttonEntity->texture)
		{
			SDL_Log("Unable to create background texture: %s", SDL_GetError());
			free(buttonEntity);
			return;
		}
	}
	else
	{
		buttonEntity->texture = backgroundTexture;

		int width;
		int height;

		SDL_QueryTexture(backgroundTexture, NULL, NULL, &width, &height);
		buttonEntity->originalSize = (Vector2Int){width, height};
	}

	AddToUIEntitiesDrawList(app, window, buttonEntity);

	if (text != NULL && strlen(text) > 0)
	{
		UIEntity *textEntity = CreateStaticText(text, textColor, app, window, position, textScale, buttonEntity);
		ListAdd(buttonEntity->childEntities, textEntity);
	}

	CreateGizmo(app, window, (SDL_Color){255, 0, 0, 255}, 1, buttonEntity);
}


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness,
                 UIEntity *connectedEntity)
{
	GizmoEntity *gizmoEntity = calloc(1, sizeof(GizmoEntity));
	gizmoEntity->connectedEntity = connectedEntity;
	gizmoEntity->color = color;
	gizmoEntity->thickness = thickness;

	AddToGizmoEntitiesDrawList(app, window, gizmoEntity);
}


void UpdateChildrenScale(UIEntity *uiEntity)
{
	if (uiEntity->childEntities->size > 0)
	{
		Vector2Float parentScaleDelta = (Vector2Float){
			uiEntity->parentScale.x - uiEntity->lastFrameParentScale.x,
			uiEntity->parentScale.y - uiEntity->lastFrameParentScale.y
		};

		if (parentScaleDelta.x != 0.0f || parentScaleDelta.y != 0.0f)
		{
			for (int j = 0; j < uiEntity->childEntities->size; j++)
			{
				UIEntity *child = uiEntity->childEntities->elements[j];
				child->parentScale = (Vector2Float){
					child->parentScale.x + parentScaleDelta.x,
					child->parentScale.y + parentScaleDelta.y
				};
			}
		}

		uiEntity->lastFrameParentScale = uiEntity->parentScale;
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
				child->worldPosition = (Vector2Int){
					child->worldPosition.x + parentPositionDelta.x,
					child->worldPosition.y + parentPositionDelta.y
				};
			}
		}

		uiEntity->lastFrameWorldPosition = uiEntity->worldPosition;
	}
}


void HandleInteractions(App *app, UIEntity *uiEntity)
{
	switch (uiEntity->uiType)
	{
		case BUTTON:
		{
			Vector2Int mousePosition = GetMousePosition();

			Vector2Int boundsMin = (Vector2Int){
				uiEntity->worldPosition.x - uiEntity->size.x / 2,
				uiEntity->worldPosition.y - uiEntity->size.y / 2
			};
			Vector2Int boundsMax = (Vector2Int){
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
					if (uiEntity->OnInteraction != NULL)
					{
						uiEntity->OnInteraction(app, uiEntity->interactionData);
					}
					if (uiEntity->OnInteractionAnimation != NULL)
					{
						uiEntity->OnInteractionAnimation(app, uiEntity);
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

			break;
		}


		default:
			break;
	}
}


void UpdateUIElements(void *data, App *app, float deltaTime)
{
	for (int i = 0; i < app->allUIEntities->size; i++)
	{
		UIEntity *uiEntity = app->allUIEntities->elements[i];

		UpdateChildrenScale(uiEntity);
		UpdateChildrenPosition(uiEntity);
		HandleInteractions(app, uiEntity);
	}
}


Updatable *CreateUIUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateUIElements);
	return updatable;
}
