#include <SDL_image.h>
#include "../Render/Headers/draw.h"
#include "../General/Headers/structs.h"
#include "../DataStructures/Headers/dictionary.h"
#include "../Render/Headers/openGL.h"
#include "../Infrastructure/Headers/update.h"
#include "../Utils/Headers/viewPortUtils.h"
#include "../UI/Headers/ui.h"
#include "Headers/textures.h"


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


void AddPostProcessingEffect(App *app, char *effectName)
{
	if (strcmp(effectName, "vignette") == 0)
	{
		Material *vignetteMaterial = CreateMaterial("PostProcessing/Vignette.frag", "PostProcessing/post.vert");
		DictionaryAdd(app->renderer->postProcessingEffects, effectName, vignetteMaterial);
	}
	else if (strcmp(effectName, "wobble") == 0)
	{
		Material *wobbleMaterial = CreateMaterial("PostProcessing/wobble.frag", "PostProcessing/post.vert");
		AddUniformToMaterial(wobbleMaterial, "time", UNIFORM_FLOAT, &app->time);
		DictionaryAdd(app->renderer->postProcessingEffects, effectName, wobbleMaterial);
	}
	else
	{
		printf("Unknown post processing effect");
	}
}


void AddGameEntityToDrawList(Window *window, GameEntity *entity)
{
	ListAdd(window->gameEntitiesDrawList, entity);
}


void AddUIEntityToDrawList(Window *window, UIEntity *entity)
{
	ListAdd(window->uiEntitiesDrawList, entity);
}


void AddGameEntityToAllDrawLists(App *app, GameEntity *entity)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];
		AddGameEntityToDrawList(window, entity);
	}
}


void AddUIEntityToAllDrawLists(App *app, UIEntity *entity)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = app->allWindows->elements[i];
		AddUIEntityToDrawList(window, entity);
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


void RenderGameEntity(App *app, Vector2Float screenPosition, GameEntity *entity, float projectionMatrix[16])
{
	entity->size.x = entity->originalSize.x * app->pixelsPerUnit * entity->scale.x;
	entity->size.y = entity->originalSize.y * app->pixelsPerUnit * entity->scale.y;

	float x = screenPosition.x - entity->size.x / 2.0f;
	float y = screenPosition.y - entity->size.y / 2.0f;

	glUseProgram(entity->material->shaderProgram);
	UniformTypeValuePair *typeValuePair = DictionaryGet(entity->material->materialUniforms, "projection");
	typeValuePair->uniformValue = projectionMatrix;
	UpdateMaterialUniforms(entity->material);
	RenderTexture(app->renderer, entity->texture, x, y, entity->size.x, entity->size.y);
}


void RenderUIEntity(App *app, Vector2Float screenPosition, UIEntity *entity, float projectionMatrix[16])
{
	entity->size.x = entity->originalSize.x * app->pixelsPerUnit * entity->scale.x * entity->parentScale.x;
	entity->size.y = entity->originalSize.y * app->pixelsPerUnit * entity->scale.y * entity->parentScale.y;


	float x = screenPosition.x - entity->size.x / 2.0f;
	float y = screenPosition.y - entity->size.y / 2.0f;

	glUseProgram(entity->material->shaderProgram);
	UniformTypeValuePair *typeValuePair = DictionaryGet(entity->material->materialUniforms, "projection");
	typeValuePair->uniformValue = projectionMatrix;
	UpdateMaterialUniforms(entity->material);
	RenderTexture(app->renderer, entity->texture, x, y, entity->size.x, entity->size.y);
}


void DrawGizmoRect(Renderer *renderer, float x, float y, float width, float height)
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


void RenderDynamicText(TextAtlas *textAtlas, char *text, Vector2Float position, Vector2Float scale, Renderer *renderer,
                       float projectionMatrix[16])
{
	float currentX = 0;

	glUseProgram(renderer->defaultMaterial->shaderProgram);
	UniformTypeValuePair *typeValuePair = DictionaryGet(renderer->defaultMaterial->materialUniforms, "projection");
	typeValuePair->uniformValue = projectionMatrix;
	UpdateMaterialUniforms(renderer->defaultMaterial);
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


void RenderHollowGizmoRect(App *app, Vector2Float position, Vector2Float size, float thickness)
{
	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y - size.y / 2, size.x, thickness);

	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y + size.y / 2, size.x, thickness);

	DrawGizmoRect(app->renderer, position.x - size.x / 2, position.y - size.y / 2, thickness,
	              size.y);

	DrawGizmoRect(app->renderer, position.x + size.x / 2 - thickness, position.y - size.y / 2 + thickness, thickness,
	              size.y);
}


void RenderDebugInfo(App *app, float projectionMatrix[16])
{
	char fpsText[30];
	snprintf(fpsText, sizeof(fpsText), "FPS:%.1f", app->debugData.fps);

	RenderDynamicText(app->textAtlas, fpsText, (Vector2Float){10, 10}, (Vector2Float){1, 1}, app->renderer,
	                  projectionMatrix);
}


void RenderGizmo(App *app, GizmoEntity *gizmoEntity, float projectionMatrix[16])
{
	glUseProgram(app->renderer->defaultGizmoMaterial->shaderProgram);
	UniformTypeValuePair *typeValuePair = DictionaryGet(app->renderer->defaultGizmoMaterial->materialUniforms,
	                                                    "projection");
	typeValuePair->uniformValue = projectionMatrix;
	UpdateMaterialUniforms(app->renderer->defaultGizmoMaterial);
	GLint colorLocation = glGetUniformLocation(app->renderer->defaultGizmoMaterial->shaderProgram, "color");
	glUniform4f(colorLocation,
	            gizmoEntity->color.r / 255.0f,
	            gizmoEntity->color.g / 255.0f,
	            gizmoEntity->color.b / 255.0f,
	            gizmoEntity->color.a / 255.0f);


	RenderHollowGizmoRect(app, gizmoEntity->connectedEntity->worldPosition, gizmoEntity->connectedEntity->size,
	                      gizmoEntity->thickness);

	char entityInfo[150];
	snprintf(entityInfo, sizeof(entityInfo), "x:%.1f,y:%.1f w:%.1f,h:%.1f",
	         gizmoEntity->connectedEntity->worldPosition.x,
	         gizmoEntity->connectedEntity->worldPosition.y,
	         gizmoEntity->connectedEntity->size.x,
	         gizmoEntity->connectedEntity->size.y);

	RenderDynamicText(app->textAtlas, entityInfo, (Vector2Float){
		                  gizmoEntity->connectedEntity->worldPosition.x - gizmoEntity->connectedEntity->size.x / 2,
		                  gizmoEntity->connectedEntity->worldPosition.y - gizmoEntity->connectedEntity->size.y / 2 - 11
	                  }, (Vector2Float){1, 1}, app->renderer, projectionMatrix);
}


void PrepareScene(Window *window, App *app)
{
	SDL_GL_MakeCurrent(window->sdlWindow, app->glContext);
	glBindFramebuffer(GL_FRAMEBUFFER, window->FBO);
	glViewport(0, 0, window->size.x, window->size.y);
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


Updatable *CreateRenderUpdatable()
{
	Updatable *updatable = CreateUpdatable(NULL, UpdateRenderer);
	return updatable;
}


void ApplyPostProcessing(App *app, Window *window)
{
	if (app->renderer->postProcessingEffects->totalEntries == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(app->renderer->defaultPostProcessingMaterial->shaderProgram);

		glBindVertexArray(app->renderer->postProcessingVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, window->FBOTexture);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else
	{
		for (int i = 0; i < app->renderer->postProcessingEffects->allPairs->size; i++)
		{
			if (i == app->renderer->postProcessingEffects->allPairs->size - 1)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, window->FBO);
			}


			KeyValuePair *pair = DictionaryGetPair(app->renderer->postProcessingEffects, i);
			Material *material = pair->value;

			UpdateMaterialUniforms(material);

			glUseProgram(material->shaderProgram);

			glBindVertexArray(app->renderer->postProcessingVAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, window->FBOTexture);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}


void UpdateMaterialUniforms(Material *material)
{
	for (int i = 0; i < material->materialUniforms->allPairs->size; i++)
	{
		KeyValuePair *pair = DictionaryGetPair(material->materialUniforms, i);
		char *uniformName = pair->key;
		UniformTypeValuePair *typeValuePair = pair->value;

		SetShaderUniform(material->shaderProgram, uniformName, typeValuePair->uniformType, typeValuePair->uniformValue);
	}
}


void UpdateRenderer(void *data, App *app, float deltaTime)
{
	for (int i = 0; i < app->allWindows->size; i++)
	{
		Window *window = ListGet(app->allWindows, i);

		float projectionMatrix[16];

		CalculateProjectionMatrix(projectionMatrix, window->size.x, window->size.y);

		PrepareScene(window, app);

		glBindVertexArray(app->renderer->entitiesVAO);

		for (int j = 0; j < window->gameEntitiesDrawList->size; j++)
		{
			GameEntity *entity = ListGet(window->gameEntitiesDrawList, j);

			Vector2Float screenPos;

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

		for (int j = 0; j < window->uiEntitiesDrawList->size; j++)
		{
			UIEntity *entity = ListGet(window->uiEntitiesDrawList, j);
			Vector2Float screenPos = entity->worldPosition;
			RenderUIEntity(app, screenPos, entity, projectionMatrix);
		}


		if (app->debugMode)
		{
			glBindVertexArray(app->renderer->gizmosVAO);

			for (int k = 0; k < window->gizmosEntitiesDrawList->size; k++)
			{
				GizmoEntity *gizmoEntity = ListGet(window->gizmosEntitiesDrawList, k);
				RenderGizmo(app, gizmoEntity, projectionMatrix);
			}

			// glBindVertexArray(app->renderer->entitiesVAO);

			RenderDebugInfo(app, projectionMatrix);
		}

		ApplyPostProcessing(app, window);

		SDL_GL_SwapWindow(window->sdlWindow);
	}
}
