#include "Headers/dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUCKETS_NUMBER 101
#define MAX_LOAD_FACTOR 0.75

void DictionaryCheckAndResize(Dictionary *dict);


Dictionary *DictionaryCreate(unsigned int (*hashFunction)(void *key), bool (*keyEquals)(void *key1, void *key2))
{
	Dictionary *dict = calloc(1, sizeof(Dictionary));

	if (dict == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for dictionary\n");
		return NULL;
	}

	List **buckets = calloc(DEFAULT_BUCKETS_NUMBER, sizeof(List));

	if (buckets == NULL)
	{
		free(dict);
		fprintf(stderr, "Failed to allocate memory for dictionary pairs\n");
		return NULL;
	}


	for (int i = 0; i < DEFAULT_BUCKETS_NUMBER; i++)
	{
		List *bucket = ListCreate(0);

		if (bucket == NULL)
		{
			for (int j = 0; j < DEFAULT_BUCKETS_NUMBER; j++)
			{
				ListDestroy(buckets[j]);
			}

			fprintf(stderr, "Failed to allocate memory for dictionary buckets\n");
			free(buckets);
			return NULL;
		}

		buckets[i] = bucket;
	}

	dict->allPairs = ListCreate(0);
	dict->bucketsNumber = DEFAULT_BUCKETS_NUMBER;
	dict->buckets = buckets;
	dict->hashFunction = hashFunction;
	dict->keyEquals = keyEquals;
	dict->totalEntries = 0;

	return dict;
}


bool DictionaryAdd(Dictionary *dict, void *key, void *value)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		fprintf(stderr, "Key is NULL\n");
		return false;
	}

	if (value == NULL)
	{
		fprintf(stderr, "Value is NULL\n");
		return false;
	}

	unsigned int hash = dict->hashFunction(key);

	int bucketIndex = hash % dict->bucketsNumber;

	KeyValuePair *pair = calloc(1, sizeof(KeyValuePair));
	pair->key = key;
	pair->value = value;
	dict->totalEntries++;

	List *bucket = dict->buckets[bucketIndex];


	if (!ListAdd(bucket, pair))
	{
		fprintf(stderr, "Failed to add pair to dictionary\n");
		free(pair);
		return false;
	}

	if (bucket->size > 1)
	{
		DictionaryCheckAndResize(dict);
	}

	ListAdd(dict->allPairs, pair);

	return true;
}


void DictionaryDestroy(Dictionary *dict)
{
	if (dict == NULL)
	{
		return;
	}

	for (int i = 0; i < dict->bucketsNumber; i++)
	{
		List *bucket = dict->buckets[i];
		for (int j = 0; j < bucket->size; j++)
		{
			KeyValuePair *pair = bucket->elements[j];
			free(pair);
		}

		ListDestroy(bucket);
	}

	ListDestroy(dict->allPairs);

	free(dict->buckets);
	free(dict);
}


bool DictionaryHasKey(Dictionary *dict, void *key)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		fprintf(stderr, "Key is NULL\n");
		return false;
	}

	unsigned int hash = dict->hashFunction(key);

	int bucketIndex = hash % dict->bucketsNumber;

	List *bucket = dict->buckets[bucketIndex];

	for (int i = 0; i < bucket->size; i++)
	{
		KeyValuePair *pair = bucket->elements[i];

		if (dict->keyEquals(pair->key, key))
		{
			return true;
		}
	}

	return false;
}


bool ResizeDictionary(Dictionary *dict, int newSize)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return false;
	}

	List **newBuckets = malloc(sizeof(List) * newSize);

	if (newBuckets == NULL)
	{
		fprintf(stderr, "Failed to reallocate memory for dictionary buckets\n");
		return false;
	}

	for (int i = 0; i < newSize; i++)
	{
		newBuckets[i] = ListCreate(0);

		if (newBuckets[i] == NULL)
		{
			for (int j = 0; j < newSize; j++)
			{
				ListDestroy(newBuckets[j]);
			}

			fprintf(stderr, "Failed to allocate memory for dictionary buckets\n");
			free(newBuckets);
			return false;
		}
	}

	for (int i = 0; i < dict->bucketsNumber; i++)
	{
		List *bucket = dict->buckets[i];

		for (int j = 0; j < bucket->size; j++)
		{
			KeyValuePair *pair = bucket->elements[j];

			unsigned int hash = dict->hashFunction(pair->key);
			unsigned int newBucketIndex = hash % newSize;

			ListAdd(newBuckets[newBucketIndex], pair);
		}

		ListDestroy(bucket);
	}

	free(dict->buckets);

	dict->buckets = newBuckets;
	dict->bucketsNumber = newSize;

	return true;
}


void DictionaryCheckAndResize(Dictionary *dict)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return;
	}

	float filledBucketsPercentage = (float) dict->totalEntries / (float) dict->bucketsNumber;

	if (filledBucketsPercentage >= MAX_LOAD_FACTOR)
	{
		ResizeDictionary(dict, dict->bucketsNumber * 2);
	}
}


void DictionaryChangeValue(Dictionary *dict, void *key, void *value)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return;
	}

	if (key == NULL)
	{
		fprintf(stderr, "Key is NULL\n");
		return;
	}

	unsigned int hash = dict->hashFunction(key);

	int bucketIndex = hash % dict->bucketsNumber;

	List *bucket = dict->buckets[bucketIndex];

	for (int i = 0; i < bucket->size; i++)
	{
		KeyValuePair *pair = bucket->elements[i];

		if (dict->keyEquals(pair->key, key))
		{
			pair->value = value;
			return;
		}
	}
}


void *DictionaryGet(Dictionary *dict, void *key)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return NULL;
	}

	if (key == NULL)
	{
		fprintf(stderr, "Key is NULL\n");
		return NULL;
	}

	unsigned int hash = dict->hashFunction(key);
	unsigned int bucketIndex = hash % dict->bucketsNumber;

	List *bucket = dict->buckets[bucketIndex];

	for (int i = 0; i < bucket->size; i++)
	{
		KeyValuePair *pair = bucket->elements[i];

		if (dict->keyEquals(pair->key, key))
		{
			return pair->value;
		}
	}

	fprintf(stderr, "Key not found\n");
	return NULL;
}


KeyValuePair *DictionaryGetPair(Dictionary *dict, int index)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return NULL;
	}

	return ListGet(dict->allPairs, index);
}


void DictionaryRemove(Dictionary *dict, void *key)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return;
	}

	if (key == NULL)
	{
		fprintf(stderr, "Key is NULL\n");
		return;
	}

	unsigned int hash = dict->hashFunction(key);
	unsigned int bucketIndex = hash % dict->bucketsNumber;

	List *bucket = dict->buckets[bucketIndex];

	for (int i = 0; i < bucket->size; i++)
	{
		KeyValuePair *pair = bucket->elements[i];

		if (dict->keyEquals(pair->key, key))
		{
			ListRemoveAtIndex(bucket, i);

			for (int j = 0; j < dict->allPairs->size; j++)
			{
				if (dict->allPairs->elements[j] == pair)
				{
					ListRemoveAtIndex(dict->allPairs, j);
					break;
				}
			}

			free(pair);
			dict->totalEntries--;
			return;
		}
	}

	fprintf(stderr, "Key not found in dictionary\n");
}


void DictionaryClear(Dictionary *dict)
{
	if (dict == NULL)
	{
		fprintf(stderr, "Dictionary is NULL\n");
		return;
	}

	for (int i = 0; i < dict->allPairs->size; i++)
	{
		KeyValuePair *pair = dict->allPairs->elements[i];
		free(pair);
	}

	for (int i = 0; i < dict->bucketsNumber; i++)
	{
		ListClear(dict->buckets[i]);
	}

	ListClear(dict->allPairs);
	dict->totalEntries = 0;
}


unsigned int HashInt(void *key)
{
	int keyValue = *(int *) key;
	unsigned int hash = keyValue * 2654435761u;
	return hash;
}


bool IntEquals(void *key1, void *key2)
{
	return *(int *) key1 == *(int *) key2;
}


unsigned int HashPointer(void *key)
{
	uintptr_t address = (uintptr_t) key;
	unsigned int hash = address * 2654435761u;
	return hash;
}


bool PointerEquals(void *key1, void *key2)
{
	return (uintptr_t) key1 == (uintptr_t) key2;
}


unsigned int HashString(void *key)
{
	unsigned long hash = 5381;
	int c;
	char *str = key;

	while ((c = *str++))
	{
		hash = (hash << 5) + hash + c;
	}

	return (unsigned int) hash;
}


bool StringEquals(void *key1, void *key2)
{
	return strcmp(key1, key2) == 0;
}
