#include "Headers/ui.h"

#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#include "Headers/app.h"
#include "Headers/draw.h"
#include "Headers/input.h"
#include "Headers/structs.h"
#include "Headers/window.h"
#include "Utils/mathUtils.h"


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness, UIEntity *connectedEntity);


void DrawDynamicText(Window *window, TextAtlas *textAtlas, char *text, Vector2Int position, Vector2Float scale)
{
	int currentX = 0;
	SDL_Texture *atlasTexture = GetFromDictionary(textAtlas->windowTexturesDictionary, window);

	for (int i = 0; text[i] != '\0'; i++)
	{
		unsigned char character = (unsigned char) text[i];

		if (textAtlas->characterRects[character].w > 0 && textAtlas->characterRects[character].h > 0)
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

	SDL_Rect bottomRect = {position.x - size.x / 2, position.y + size.y / 2 - thickness, size.x, thickness};
	SDL_RenderFillRect(window->renderer, &bottomRect);

	SDL_Rect leftRect = {
		position.x - size.x / 2, position.y - size.y / 2 + thickness, thickness, size.y - 2 * thickness
	};
	SDL_RenderFillRect(window->renderer, &leftRect);

	SDL_Rect rightRect = {
		position.x + size.x / 2 - thickness, position.y - size.y / 2 + thickness, thickness, size.y - 2 * thickness
	};
	SDL_RenderFillRect(window->renderer, &rightRect);
}


TextAtlas *CreateTextAtlas(char *fontPath, int fontSize)
{
	TextAtlas *textAtlas = calloc(1, sizeof(TextAtlas));
	memset(textAtlas->characterRects, 0, sizeof(textAtlas->characterRects));
	textAtlas->windowTexturesDictionary = CreateDictionary(HashWindow, WindowEquals);

	TTF_Font *font = TTF_OpenFont(fontPath, fontSize);

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

		SDL_Surface *characterSurface = TTF_RenderText_Blended(font, text, (SDL_Color){255, 255, 255, 255});

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
	SDL_Texture *atlasTexture = SDL_CreateTextureFromSurface(window->renderer, textAtlas->atlasSurface);
	AddToDictionary(textAtlas->windowTexturesDictionary, window, atlasTexture);
}


UIEntity *CreateStaticText(char *text, int fontSize, SDL_Color textColor, App *app, Window *window, Vector2Int position,
                           Vector2Float scale, UIEntity *parent)
{
	UIEntity *textEntity = calloc(1, sizeof(UIEntity));

	textEntity->scale = scale;
	textEntity->parentEntity = parent;

	TTF_Font *font = TTF_OpenFont("D:/CWindowGame/Assets/ByteBounce.ttf", fontSize);
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


	textEntity->worldPosition = position;

	textEntity->uiType = TEXT;
	textEntity->originalSize = (Vector2Int){textWidth, textHeight};

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	AddToUIEntitiesDrawList(app, window, textEntity);

	return textEntity;
}


void CreateButton(SDL_Texture *backgroundTexture, Vector2Int size, SDL_Color backgroundColor, char *text, int fontSize,
                  SDL_Color textColor, App *app, Window *window, Vector2Int position, Vector2Float scale,
                  void (*OnInteraction)(), void (*OnInteractionAnimation)(App *app, UIEntity *uiEntity),
                  UIEntity *parent)
{
	UIEntity *buttonEntity = calloc(1, sizeof(UIEntity));
	buttonEntity->scale = scale;
	buttonEntity->uiType = BUTTON;
	buttonEntity->childEntities = CreateList(0);
	buttonEntity->parentEntity = parent;
	buttonEntity->OnInteraction = OnInteraction;
	buttonEntity->OnInteractionAnimation = OnInteractionAnimation;

	buttonEntity->worldPosition = position;

	if (backgroundTexture == NULL)
	{
		SDL_Surface *buttonSurface = SDL_CreateRGBSurface(0, size.x, scale.y, 32, 0, 0, 0, 0);
		if (!buttonSurface)
		{
			SDL_Log("Unable to create rectangle surface: %s", SDL_GetError());
			free(buttonEntity);
			return;
		}

		SDL_FillRect(buttonSurface, NULL,
		             SDL_MapRGB(buttonSurface->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));

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
		UIEntity *textEntity = CreateStaticText(text, fontSize, textColor, app, window, position, scale, buttonEntity);
		AddToList(buttonEntity->childEntities, textEntity);
	}

	CreateGizmo(app, window, (SDL_Color){255, 0, 0, 255}, 2, buttonEntity);
}


void CreateGizmo(App *app, Window *window, SDL_Color color, int thickness, UIEntity *connectedEntity)
{
	GizmoEntity *gizmoEntity = calloc(1, sizeof(GizmoEntity));
	gizmoEntity->connectedEntity = connectedEntity;
	gizmoEntity->color = color;
	gizmoEntity->thickness = thickness;

	AddToGizmoEntitiesDrawList(app, window, gizmoEntity);
}


void UpdateUIElements(App *app)
{
	for (int i = 0; i < app->allUIEntities->size; i++)
	{
		UIEntity *uiEntity = app->allUIEntities->elements[i];

		switch (uiEntity->uiType)
		{
			case BUTTON:
			{
				Vector2Int mousePosition = GetMousePosition();

				Vector2Int boundsMin = (Vector2Int){
					uiEntity->worldPosition.x - uiEntity->size.x / 2, uiEntity->worldPosition.y - uiEntity->size.y / 2
				};
				Vector2Int boundsMax = (Vector2Int){
					uiEntity->worldPosition.x + uiEntity->size.x / 2,
					uiEntity->worldPosition.y + uiEntity->size.y / 2
				};

				if (IsPointInBounds(mousePosition, boundsMin, boundsMax))
				{
					if (IsLeftMouseButtonClicked())
					{
						if (uiEntity->OnInteraction != NULL)
						{
							uiEntity->OnInteraction();
						}
						if (uiEntity->OnInteractionAnimation != NULL)
						{
							uiEntity->OnInteractionAnimation(app, uiEntity);
						}
					}
				}

				break;
			}


			default:
				break;
		}
	}
}
