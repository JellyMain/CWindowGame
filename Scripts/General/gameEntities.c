#include "Headers/gameEntities.h"

#include "../Render/Headers/draw.h"
#include "Headers/structs.h"


GameEntity *CreateGameEntity(App *app, Texture *texture, Material *material, Vector2Float position, Vector2Float scale)
{
	GameEntity *gameEntity = calloc(1, sizeof(GameEntity));

	gameEntity->texture = texture;
	gameEntity->material = material;

	gameEntity->worldPosition = position;
	gameEntity->scale = scale;
	gameEntity->originalSize.x = gameEntity->texture->width;
	gameEntity->originalSize.y = gameEntity->texture->height;

	ListAdd(app->allGameEntities, gameEntity);
	AddGameEntityToAllDrawLists(app, gameEntity);

	return gameEntity;
}
