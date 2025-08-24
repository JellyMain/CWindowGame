#include "Headers/List.h"

#include <stdio.h>
#include <stdlib.h>

List *CreateList(int capacity)
{
	if (capacity <= 0)
	{
		capacity = DEFAULT_CAPACITY;
	}

	List *list = malloc(sizeof(List));

	if (list == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for list\n");
		return NULL;
	}

	list->elements = (void **) malloc(sizeof(void *) * capacity);

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


void DestroyList(List *list)
{
	if (list == NULL)
	{
		return;
	}

	ClearList(list);

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


bool AddToList(List *list, void *element)
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


void *GetFromList(List *list, int index)
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


bool SetToList(List *list, int index, void *element)
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


bool RemoveFromListAtIndex(List *list, int index)
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


int GetListSize(List *list)
{
	if (list == NULL)
	{
		fprintf(stderr, "List is NULL\n");
		return 0;
	}

	return list->size;
}


bool IsListEmpty(List *list)
{
	if (list == NULL || list->size == 0)
	{
		return true;
	}

	return false;
}


void ClearList(List *list)
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
