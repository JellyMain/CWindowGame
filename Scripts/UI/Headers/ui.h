#pragma once
#include "../../General/Headers/structs.h"


UIEntity *CreateStaticText(Window *window, App *app, char *text, SDL_Color textColor,
                           Vector2Float position,
                           Vector2Float scale,
                           UIEntity *parent);

UIEntity *CreateButton(Window *window, Texture *buttonTexture, Material *buttonMaterial,
                       Vector2Float size,
                       SDL_Color backgroundColor,
                       char *text, Vector2Float textScale,
                       SDL_Color textColor, App *app, Vector2Float position,
                       Vector2Float buttonScale, UIInteraction *uiInteraction,
                       void (*OnHover)(App *app, UIEntity *uiEntity),
                       void (*OnHoverExit)(App *app, UIEntity *uiEntity),
                       UIEntity *parent);


TextAtlas *CreateTextAtlas(char *fontPath, int fontSize);

UIInteraction *CreateUIInteraction(void *interactionData,
                                   void (*OnInteraction)(App *app, void *data),
                                   void (*OnInteractionAnimation)(
	                                   App *app, UIEntity *uiEntity));

UIEntity *CreateInputField(Window *window, Texture *inputFieldTexture,
                           Material *inputFieldMaterial,
                           Vector2Float size,
                           SDL_Color backgroundColor, App *app, Vector2Float position,
                           Vector2Float inputFieldScale, Vector2Float textScale, UIEntity *parent);

InputFieldData *CreateInputFieldData(UIEntity *inputFieldEntity, int maxLength);

void UpdateWindow(App *app, Window *window);

Updatable *CreateUIUpdatable();
