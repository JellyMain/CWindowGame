#pragma once
#include "../DataStructures/Headers/List.h"
#include "../DataStructures/Headers/Dictionary.h"

typedef struct TextAtlas TextAtlas;


typedef struct
{
	List *allGameEntities;
	List *allUIEntities;
	List *allGizmosEntities;
	Dictionary *gizmosEntitiesDrawDictionary;
	Dictionary *gameEntitiesDrawDictionary;
	Dictionary *uiEntitiesDrawDictionary;
	TextAtlas *textAtlas;
	bool hasWon;
	bool showGizmos;
} App;


App *CreateApp(bool showGizmos);
