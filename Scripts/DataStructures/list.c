#include "Headers/list.h"

#include <stdio.h>
#include <stdlib.h>

List *ListCreate(int capacity)
{
	if (capacity <= 0)
	{
		capacity = DEFAULT_CAPACITY;
	}

	List *list = calloc(1, sizeof(List));

	if (list == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for list\n");
		return NULL;
	}

	list->elements = (void **) calloc(capacity, sizeof(void *));

	if (list->elements == NULL)
	{
		free(list);
		fprintf(stderr, "Failed to allocate memory for list elements\n");
		return NULL;
	}

	list->size = 0;
	list->capacity = capacity;

	return list;
}


void ListDestroy(List *list)
{
	if (list == NULL)
	{
		return;
	}

	ListClear(list);

	free(list->elements);
	free(list);
}


bool ListHasEnoughCapacity(List *list, int minCapacity)
{
	if (minCapacity <= list->capacity)
	{
		return true;
	}

	int newCapacity = list->capacity * GROWTH_FACTOR;

	if (newCapacity < minCapacity)
	{
		newCapacity = minCapacity;
	}


	void **newElements = realloc(list->elements, sizeof(void *) * newCapacity);

	if (newElements == NULL)
	{
		fprintf(stderr, "Failed to reallocate memory for list elements\n");
		return false;
	}

	list->elements = newElements;
	list->capacity = newCapacity;

	return true;
}


bool ListAdd(List *list, void *element)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return false;
	}

	if (!ListHasEnoughCapacity(list, list->size + 1))
	{
		fprintf(stderr, "Failed to check capacity\n");
		return false;
	}

	list->elements[list->size] = element;
	list->size++;

	return true;
}


void *ListGet(List *list, int index)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return NULL;
	}

	if (index < 0 || index >= list->size)
	{
		fprintf(stderr, "Index out of bounds\n");
	}

	return list->elements[index];
}


bool ListSet(List *list, int index, void *element)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return NULL;
	}

	if (index < 0 || index >= list->size)
	{
		fprintf(stderr, "Index out of bounds\n");
		return false;
	}


	list->elements[index] = element;
	return true;
}


bool ListRemoveAtIndex(List *list, int index)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return false;
	}

	if (index < 0 || index >= list->size)
	{
		fprintf(stderr, "Index out of bounds\n");
		return false;
	}

	list->elements[index] = NULL;

	for (int i = index; i < list->size - 1; i++)
	{
		void *nextElement = list->elements[i + 1];
		list->elements[i] = nextElement;
	}

	list->size--;

	return true;
}


bool ListRemove(List *list, void *element)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return false;
	}

	int index = -1;

	for (int i = list->size - 1; i >= 0; i--)
	{
		if (list->elements[i] == element)
		{
			list->elements[i] = NULL;
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		fprintf(stderr, "Element was not found in the list\n");
		return false;
	}


	for (int i = index; i < list->size - 1; i++)
	{
		void *nextElement = list->elements[i + 1];
		list->elements[i] = nextElement;
	}

	list->size--;
	return true;
}


int ListGetSize(List *list)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return 0;
	}

	return list->size;
}


bool ListIsEmpty(List *list)
{
	if (list == NULL || list->size == 0)
	{
		return true;
	}

	return false;
}


void ListClear(List *list)
{
	if (list == NULL)
	{
		return;
	}

	for (int i = 0; i < list->size; i++)
	{
		list->elements[i] = NULL;
	}

	list->size = 0;
}
