#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#define DEFAULT_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct
{
	void **elements;
	int size;
	int capacity;
} List;


typedef struct
{
	int size;
	int capacity;
	size_t elementSize;
	const char *elementsTypeName;
} ListMetaData;

#define MacroListCreate(T, capacity) (T*)NewListCreate(sizeof(T), #T, capacity)

#define MacroListAdd(list, element) do {\
	(void)sizeof((list)[0] = (element));\
	list = (typeof(list)) CheckAndIncreaseListCapacity(list);\
	ListMetaData *listMetaData = GetListMetaData(list);\
	list[listMetaData->size] = element;\
	listMetaData->size++;\
	}while (0)


#define MacroListDestroy(list) do {\
	ListMetaData *listMetaData = GetListMetaData(list);\
	free(listMetaData);\
	list = NULL;\
}while (0)

#define MacroListSize(list) (GetListMetaData(list)->size)


#define MacroListRemove(list, element) do{\
	(void)sizeof((list)[0] = (element));\
	ListMetaData *listMetaData = GetListMetaData(list);\
	int index = -1;\
	for (int i = 0; i < listMetaData->size; i++)\
	{\
		if(element == list[i])\
		{\
			index = i;\
			for (int y = index + 1; y < listMetaData->size; y++)\
			{\
				list[y-1] = list[y];\
			}\
			listMetaData->size--;\
			break;\
		}\
	}\
	if (index == -1)\
	{\
		printf("Couldn't file element in the list");\
	}\
}while (0)


ListMetaData *GetListMetaData(void *list);

void *CheckAndIncreaseListCapacity(void *list);

void *NewListCreate(size_t elementSize, char *elementsTypeName, int capacity);

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
