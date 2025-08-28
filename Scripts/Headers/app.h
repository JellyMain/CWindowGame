#pragma once
#include "../DataStructures/Headers/List.h"
#include "../DataStructures/Headers/Dictionary.h"

typedef struct
{
	List *allEntities;
	Dictionary *drawDictionary;
	bool hasWon;
} App;


App *CreateApp();
