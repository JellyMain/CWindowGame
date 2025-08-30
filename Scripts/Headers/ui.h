#pragma once
#include "app.h"
#include "structs.h"
#include "window.h"


typedef struct TextAtlas
{
	SDL_Rect characterRects[256];
	int charWidth;
	int charHeight;
	int charsPerRow;
	SDL_Surface *atlasSurface;
	Dictionary *windowTexturesDictionary;
} TextAtlas;


UIEntity *CreateStaticText(char *text, int fontSize, SDL_Color textColor, App *app, Window *window, Vector2Int position,
                     Vector2Float scale, UIEntity *parent);

void CreateButton(SDL_Texture *backgroundTexture, Vector2Int size, SDL_Color backgroundColor, char *text, int fontSize,
                  SDL_Color textColor, App *app, Window *window, Vector2Int position, Vector2Float scale,
                  void (*OnInteraction)(),
                  UIEntity *parent);

void UpdateUIElements(App *app);

TextAtlas *CreateTextAtlas(char *fontPath, int fontSize);

void AddAtlasTexture(TextAtlas *textAtlas, Window *window);
