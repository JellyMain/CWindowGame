#pragma once

#include "../../General/Headers/structs.h"


Updatable *CreateRenderUpdatable();

void UpdateRenderer(void *data, App *app, float deltaTime);

void UpdateMaterialUniforms(Material *material);

void SetShaderUniform(GLuint shaderProgram, char *propertyName, UniformType uniformType, void *value);

void DrawGizmoRect(Renderer *renderer, float x, float y, float width, float height);

void RenderHollowGizmoRect(App *app, Vector2Float position, Vector2Float size, float thickness);

void RenderDynamicText(TextAtlas *textAtlas, char *text, Vector2Float position, Vector2Float scale, Renderer *renderer,
                       float projectionMatrix[16]);

void AddPostProcessingEffect(App *app, char *effectName);

void AddGameEntityToDrawList(Window *window, GameEntity *entity);

void AddUIEntityToDrawList(Window *window, UIEntity *entity);

void AddGameEntityToAllDrawLists(App *app, GameEntity *entity);

void AddUIEntityToAllDrawLists(App *app, UIEntity *entity);
