#pragma once
#include "structs.h"


UIEntity *CreateStaticText(char *text, int fontSize, SDL_Color textColor, App *app, Window *window, Vector2Int position,
                           Vector2Float scale, UIEntity *parent);

void CreateButton(SDL_Texture *backgroundTexture, Vector2Int size, SDL_Color backgroundColor, char *text, int fontSize,
				  SDL_Color textColor, App *app, Window *window, Vector2Int position, Vector2Float scale,
				  void (*OnInteraction)(), void (*OnInteractionAnimation)(App *app, UIEntity *uiEntity), UIEntity *parent);

void UpdateUIElements(App *app);

TextAtlas *CreateTextAtlas(char *fontPath, int fontSize);

void AddAtlasTexture(TextAtlas *textAtlas, Window *window);

void DrawThickRectBorder(Window *window, Vector2Int position, Vector2Int size, int thickness);

void DrawDynamicText(Window *window, TextAtlas *textAtlas, char *text, Vector2Int position, Vector2Float scale);
