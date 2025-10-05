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


List *ListCreate(int capacity);

void ListDestroy(List *list);

bool ListAdd(List *list, void *element);

void *ListGet(List *list, int index);

bool ListSet(List *list, int index, void *element);

bool ListRemoveAtIndex(List *list, int index);

bool ListRemove(List *list, void *element);

int ListGetSize(List *list);

bool ListIsEmpty(List *list);

void ListClear(List *list);
