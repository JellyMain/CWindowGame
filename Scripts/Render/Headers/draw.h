#pragma once

#include "../../General/Headers/structs.h"


Updatable *CreateRenderUpdatable();

void UpdateRenderer(void *data, App *app, float deltaTime);

void SetShaderUniform(GLuint shaderProgram, char *propertyName, UniformType uniformType, void *value);

void DrawGizmoRect(Renderer *renderer, int x, int y, int width, int height);

void RenderHollowGizmoRect(App *app, Vector2Int position, Vector2Int size, int thickness);

void RenderDynamicText(TextAtlas *textAtlas, char *text, Vector2Int position, Vector2Float scale, Renderer *renderer,
                       float projectionMatrix[16]);
