#pragma once
#include "List.h"


typedef struct
{
	List **buckets;

	int bucketsNumber;

	unsigned int totalEntries;

	unsigned int (*hashFunction)(void *key);

	bool (*keyEquals)(void *key1, void *key2);
} Dictionary;


Dictionary *CreateDictionary(unsigned int (*hashFunction)(void *key), bool (*keyEquals)(void *key1, void *key2));

void *GetFromDictionary(Dictionary *dict, void *key);

bool AddToDictionary(Dictionary *dict, void *key, void *value);

unsigned int HashInt(void *key);

bool IntEquals(void *key1, void *key2);

bool DictionaryHasKey(Dictionary *dict, void *key);

void DestroyDictionary(Dictionary *dict);

void ChangeDictionaryValue(Dictionary *dict, void *key, void *value);
