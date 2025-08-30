#include "Headers/Dictionary.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUCKETS_NUMBER 101
#define MAX_LOAD_FACTOR 0.75

void CheckAndResizeDictionary(Dictionary *dict);


Dictionary *CreateDictionary(unsigned int (*hashFunction)(void *key), bool (*keyEquals)(void *key1, void *key2))
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
		List *bucket = CreateList(0);

		if (bucket == NULL)
		{
			for (int j = 0; j < DEFAULT_BUCKETS_NUMBER; j++)
			{
				DestroyList(buckets[j]);
			}

			fprintf(stderr, "Failed to allocate memory for dictionary buckets\n");
			free(buckets);
			return NULL;
		}

		buckets[i] = bucket;
	}

	dict->allPairs = CreateList(0);
	dict->bucketsNumber = DEFAULT_BUCKETS_NUMBER;
	dict->buckets = buckets;
	dict->hashFunction = hashFunction;
	dict->keyEquals = keyEquals;
	dict->totalEntries = 0;

	return dict;
}


bool AddToDictionary(Dictionary *dict, void *key, void *value)
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


	if (!AddToList(bucket, pair))
	{
		fprintf(stderr, "Failed to add pair to dictionary\n");
		free(pair);
		return false;
	}

	if (bucket->size > 1)
	{
		CheckAndResizeDictionary(dict);
	}

	AddToList(dict->allPairs, pair);

	return true;
}


void DestroyDictionary(Dictionary *dict)
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

		DestroyList(bucket);
	}

	DestroyList(dict->allPairs);

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
		newBuckets[i] = CreateList(0);

		if (newBuckets[i] == NULL)
		{
			for (int j = 0; j < newSize; j++)
			{
				DestroyList(newBuckets[j]);
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

			AddToList(newBuckets[newBucketIndex], pair);
		}

		DestroyList(bucket);
	}

	free(dict->buckets);

	dict->buckets = newBuckets;
	dict->bucketsNumber = newSize;

	return true;
}


void CheckAndResizeDictionary(Dictionary *dict)
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


void ChangeDictionaryValue(Dictionary *dict, void *key, void *value)
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

	if (value == NULL)
	{
		fprintf(stderr, "Value is NULL\n");
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


void *GetFromDictionary(Dictionary *dict, void *key)
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


void RemoveFromDictionary(Dictionary *dict, void *key)
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
			RemoveFromListAtIndex(bucket, i);

			for (int j = 0; j < dict->allPairs->size; j++)
			{
				if (dict->allPairs->elements[j] == pair)
				{
					RemoveFromListAtIndex(dict->allPairs, j);
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


void ClearDictionary(Dictionary *dict)
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
		ClearList(dict->buckets[i]);
	}

	ClearList(dict->allPairs);
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
