#pragma once
#include "../../General/Headers/structs.h"


UIEntity *CreateStaticText(App *app, char *text, SDL_Color textColor, Vector2Int position, Vector2Float scale,
                           UIEntity *parent);

void CreateButton(Material *butttonMaterial, Vector2Int size, SDL_Color backgroundColor,
                  char *text, Vector2Float textScale,
                  SDL_Color textColor, App *app, Window *window, Vector2Int position,
                  Vector2Float buttonScale, void *interactionData,
                  void (*OnInteraction)(App *app, void *data),
                  void (*OnInteractionAnimation)(App *app, UIEntity *uiEntity),
                  void (*OnHover)(App *app, UIEntity *uiEntity),
                  void (*OnHoverExit)(App *app, UIEntity *uiEntity),
                  UIEntity *parent);


TextAtlas *CreateTextAtlas(char *fontPath, int fontSize);


void DrawThickRectBorder(Window *window, Vector2Int position, Vector2Int size, int thickness);

void DrawDynamicText(Window *window, TextAtlas *textAtlas, char *text, Vector2Int position,
                     Vector2Float scale);

void UpdateWindow(App *app, Window *window);

Updatable *CreateUIUpdatable();
