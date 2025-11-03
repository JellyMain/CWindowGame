#include <SDL_image.h>
#include "../Render/Headers/draw.h"
#include "../General/Headers/structs.h"
#include "../DataStructures/Headers/dictionary.h"
#include "../Render/Headers/openGL.h"
#include "../Infrastructure/Headers/update.h"
#include "../Utils/Headers/viewPortUtils.h"
#include "../UI/Headers/ui.h"


void SetShaderUniform(GLuint shaderProgram, char *propertyName, UniformType uniformType, void *value)
{
	glUseProgram(shaderProgram);
	GLint uniformLocation = glGetUniformLocation(shaderProgram, propertyName);

	switch (uniformType)
	{
		case UNIFORM_FLOAT:
			glUniform1f(uniformLocation, *(float *) value);
			break;

		case UNIFORM_MAT4F:
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *) value);
			break;
	}
}


void RenderTexture(Renderer *renderer, Texture *texture, float x, float y, float width, float height)
{
	float verticesData[] = {
		x, y, 0.0f, 0.0f,
		x, y + height, 0.0f, 1.0f,
		x + width, y + height, 1.0f, 1.0f,
		x + width, y, 1.0f, 0.0f
	};

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void RenderGameEntity(App *app, Vector2Int screenPosition, GameEntity *entity, float projectionMatrix[16])
{
	entity->size.x = entity->originalSize.x * app->pixelsPerUnit * entity->scale.x;
	entity->size.y = entity->originalSize.y * app->pixelsPerUnit * entity->scale.y;

	float x = screenPosition.x - entity->size.x / 2.0f;
	float y = screenPosition.y - entity->size.y / 2.0f;

	glUseProgram(entity->material->shaderProgram);
	glUniformMatrix4fv(entity->material->projectionLocation, 1, GL_FALSE, projectionMatrix);
	RenderTexture(app->renderer, entity->material->texture, x, y, entity->size.x, entity->size.y);
}


void RenderUIEntity(App *app, Vector2Int screenPosition, UIEntity *entity, float projectionMatrix[16])
{
	entity->size.x = entity->originalSize.x * app->pixelsPerUnit * entity->entityScale.x * entity->parentScale.x;
	entity->size.y = entity->originalSize.y * app->pixelsPerUnit * entity->entityScale.y * entity->parentScale.y;

	float x = screenPosition.x - entity->size.x / 2.0f;
	float y = screenPosition.y - entity->size.y / 2.0f;

	glUseProgram(entity->material->shaderProgram);
	glUniformMatrix4fv(entity->material->projectionLocation, 1, GL_FALSE, projectionMatrix);
	RenderTexture(app->renderer, entity->material->texture, x, y, entity->size.x, entity->size.y);
}


void DrawGizmoRect(Renderer *renderer, int x, int y, int width, int height)
{
	float verticesData[] = {
		x, y,
		x, y + height,
		x + width, y + height,
		x + width, y,
	};

	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void RenderDynamicText(TextAtlas *textAtlas, char *text, Vector2Int position, Vector2Float scale, Renderer *renderer,
                       float projectionMatrix[16])
{
	int currentX = 0;

	glUseProgram(renderer->defaultShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(renderer->defaultShaderProgram, "projection"), 1, GL_FALSE,
	                   projectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textAtlas->atlasTexture->textureId);

	glBindVertexArray(renderer->entitiesVAO);

	for (int i = 0; text[i] != '\0'; i++)
	{
		unsigned char character = (unsigned char) text[i];

		if (textAtlas->characterRects[character].w > 0 && textAtlas->characterRects[character].h > 0)
		{
			SDL_Rect charRect = textAtlas->characterRects[character];

			float dstX = position.x + currentX;
			float dstY = position.y;
			float dstW = charRect.w * scale.x;
			float dstH = charRect.h * scale.y;

			float atlasWidth = textAtlas->atlasTexture->width;
			float atlasHeight = textAtlas->atlasTexture->height;

			float texLeft = charRect.x / atlasWidth;
			float texRight = (charRect.x + charRect.w) / atlasWidth;
			float texTop = charRect.y / atlasHeight;
			float texBottom = (charRect.y + charRect.h) / atlasHeight;

			float verticesData[] = {
				dstX, dstY, texLeft, texTop,
				dstX, dstY + dstH, texLeft, texBottom,
				dstX + dstW, dstY + dstH, texRight, texBottom,
				dstX + dstW, dstY, texRight, texTop
			};

			glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			currentX += charRect.w * scale.x;
		}
		else
		{
			currentX += 1;
		}
	}
}


void RenderHollowGizmoRect(App *app, Vector2Int position, Vector2Int size, int thickness)
{
	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y - size.y / 2, size.x, thickness);

	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y + size.y / 2, size.x, thickness);

	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y - size.y / 2, thickness,
	              size.y);

	DrawGizmoRect(app->renderer, position.x + size.x / 2 - thickness, position.y - size.y / 2 + thickness, thickness,
	              size.y);
}


void RenderGizmo(App *app, GizmoEntity *gizmoEntity, float projectionMatrix[16])
{
	GLint projectionLocation = glGetUniformLocation(app->renderer->gizmosShaderProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
	GLint uniformLocation = glGetUniformLocation(app->renderer->gizmosShaderProgram, "color");
	glUniform4f(uniformLocation,
	            gizmoEntity->color.r / 255.0f,
	            gizmoEntity->color.g / 255.0f,
	            gizmoEntity->color.b / 255.0f,
	            gizmoEntity->color.a / 255.0f);


	RenderHollowGizmoRect(app, gizmoEntity->connectedEntity->worldPosition, gizmoEntity->connectedEntity->size,
	                      gizmoEntity->thickness);

	char entityInfo[150];
	snprintf(entityInfo, sizeof(entityInfo), "x:%d,y:%d w:%d,h%d",
	         gizmoEntity->connectedEntity->worldPosition.x,
	         gizmoEntity->connectedEntity->worldPosition.y,
	         gizmoEntity->connectedEntity->size.x,
	         gizmoEntity->connectedEntity->size.y);

	RenderDynamicText(app->textAtlas, entityInfo, (Vector2Int){
		                  gizmoEntity->connectedEntity->worldPosition.x - gizmoEntity->connectedEntity->size.x / 2,
		                  gizmoEntity->connectedEntity->worldPosition.y - gizmoEntity->connectedEntity->size.y / 2 - 10
	                  }, (Vector2Float){1, 1}, app->renderer, projectionMatrix);
}


void PrepareScene(Window *window, App *app)
{
	SDL_GL_MakeCurrent(window->sdlWindow, app->glContext);
	glViewport(0, 0, window->size.x, window->size.y);
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


Updatable *CreateRenderUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateRenderer);
	return updatable;
}


void UpdateRenderer(void *data, App *app, float deltaTime)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];

		float projectionMatrix[16];

		CalculateProjectionMatrix(projectionMatrix, window->size.x, window->size.y);

		PrepareScene(window, app);

		glBindVertexArray(app->renderer->entitiesVAO);

		for (int j = 0; j < app->allGameEntities->size; j++)
		{
			GameEntity *entity = app->allGameEntities->elements[j];

			Vector2Int screenPos;

			switch (window->renderType)
			{
				case WINDOW_SCREEN_SPACE:
					screenPos = entity->worldPosition;
					break;

				case WINDOW_WORLD_SPACE:
					screenPos = WorldToScreen(entity->worldPosition, window->viewportOffset);
					break;

				default:
					screenPos = entity->worldPosition;
					printf("Unknown window type");
					break;
			}

			RenderGameEntity(app, screenPos, entity, projectionMatrix);
		}


		for (int j = 0; j < app->allUIEntities->size; j++)
		{
			UIEntity *entity = app->allUIEntities->elements[j];
			Vector2Int screenPos = entity->worldPosition;
			RenderUIEntity(app, screenPos, entity, projectionMatrix);
		}


		if (app->showGizmos)
		{
			glUseProgram(app->renderer->gizmosShaderProgram);
			glBindVertexArray(app->renderer->gizmosVAO);

			for (int k = 0; k < app->allGizmosEntities->size; k++)
			{
				GizmoEntity *gizmoEntity = ListGet(app->allGizmosEntities, k);
				RenderGizmo(app, gizmoEntity, projectionMatrix);
			}
		}

		SDL_GL_SwapWindow(window->sdlWindow);
	}
}
