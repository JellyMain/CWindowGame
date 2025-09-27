#pragma once
#include <stdbool.h>
#define DEFAULT_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct
{
	void **elements;
	int size;
	int capacity;
} List;


List *CreateList(int capacity);

void DestroyList(List *list);

bool AddToList(List *list, void *element);

void *GetFromList(List *list, int index);

bool SetToList(List *list, int index, void *element);

bool RemoveFromListAtIndex(List *list, int index);

bool RemoveFromList(List *list, void *element);

int GetListSize(List *list);

bool IsListEmpty(List *list);

void ClearList(List *list);
