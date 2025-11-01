#include <SDL_image.h>
#include "../Render/Headers/draw.h"
#include "../General/Headers/structs.h"
#include "../DataStructures/Headers/dictionary.h"
#include "../Render/Headers/openGL.h"
#include "../Infrastructure/Headers/update.h"
#include "../Utils/Headers/viewPortUtils.h"
#include "../UI/Headers/ui.h"


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


void DrawGizmoRect(Renderer *renderer, Vector2Int position, int thickness)
{
	float verticesData[] = {
		position.x, position.y, 0.0f, 0.0f,
		position.x, position.y + thickness, 0.0f, 1.0f,
		position.x + thickness, position.y + thickness, 1.0f, 1.0f,
		position.x + thickness, position.y, 1.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void RenderGizmo(App *app, Window *window, GizmoEntity *gizmoEntity)
{
	// SDL_SetRenderDrawColor(window->renderer, gizmoEntity->color.r, gizmoEntity->color.g, gizmoEntity->color.b,
	//                        gizmoEntity->color.a);
	//
	// DrawThickRectBorder(window, gizmoEntity->connectedEntity->worldPosition, gizmoEntity->connectedEntity->size,
	//                     gizmoEntity->thickness);
	//
	//
	// char entityInfo[150];
	// snprintf(entityInfo, sizeof(entityInfo), "x:%d,y:%d w:%d,h%d",
	//          gizmoEntity->connectedEntity->worldPosition.x,
	//          gizmoEntity->connectedEntity->worldPosition.y,
	//          gizmoEntity->connectedEntity->size.x,
	//          gizmoEntity->connectedEntity->size.y);
	//
	//
	// DrawDynamicText(window, app->textAtlas, entityInfo, (Vector2Int){
	// 	                gizmoEntity->connectedEntity->worldPosition.x - gizmoEntity->
	// 	                connectedEntity->size.x
	// 	                / 2,
	// 	                gizmoEntity->connectedEntity->worldPosition.y - gizmoEntity->
	// 	                connectedEntity->size.y
	// 	                / 2 - 10
	//                 }, (Vector2Float){1, 1});
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
			// SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
			//
			// for (int k = 0; k < gizmoEntitiesDrawList->size; k++)
			// {
			// 	GizmoEntity *gizmoEntity = gizmoEntitiesDrawList->elements[k];
			// 	BlitGizmo(app, window, gizmoEntity);
			// }
			//
			// SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_NONE);
		}

		SDL_GL_SwapWindow(window->sdlWindow);
	}
}
