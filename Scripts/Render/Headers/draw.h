#pragma once

#include "../../General/Headers/structs.h"


Updatable *CreateRenderUpdatable();

void UpdateRenderer(void *data, App *app, float deltaTime);

void SetShaderUniform(GLuint shaderProgram, char *propertyName, UniformType uniformType, void *value);

void DrawGizmoRect(Renderer *renderer, float x, float y, float width, float height);

void RenderHollowGizmoRect(App *app, Vector2Float position, Vector2Float size, float thickness);

void RenderDynamicText(TextAtlas *textAtlas, char *text, Vector2Float position, Vector2Float scale, Renderer *renderer,
                       float projectionMatrix[16]);
