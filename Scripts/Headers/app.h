#pragma once
#include "../DataStructures/Headers/List.h"

typedef struct
{
	List *windowsList;
	List *drawList;
	bool hasWon;
} App;


App *CreateApp();
