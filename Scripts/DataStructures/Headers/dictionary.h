#pragma once
#include "list.h"


typedef struct
{
	void *key;
	void *value;
} KeyValuePair;


typedef struct
{
	List **buckets;

	int bucketsNumber;

	unsigned int totalEntries;

	unsigned int (*hashFunction)(void *key);

	bool (*keyEquals)(void *key1, void *key2);

	List *allPairs;
} Dictionary;


Dictionary *DictionaryCreate(unsigned int (*hashFunction)(void *key), bool (*keyEquals)(void *key1, void *key2));

void *DictionaryGet(Dictionary *dict, void *key);

KeyValuePair *DictionaryGetPair(Dictionary *dict, int index);

bool DictionaryAdd(Dictionary *dict, void *key, void *value);

unsigned int HashInt(void *key);

bool IntEquals(void *key1, void *key2);

unsigned int HashPointer(void *key);

bool PointerEquals(void *key1, void *key2);

unsigned int HashString(void *key);

bool StringEquals(void *key1, void *key2);

bool DictionaryHasKey(Dictionary *dict, void *key);

void DictionaryDestroy(Dictionary *dict);

void DictionaryChangeValue(Dictionary *dict, void *key, void *value);

void DictionaryRemove(Dictionary *dict, void *key);

void DictionaryClear(Dictionary *dict);
