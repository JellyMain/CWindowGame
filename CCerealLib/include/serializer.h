#pragma once
#define SERIALIZABLE
#include <stddef.h>

#define SerializableArray(T) SerializableArray

typedef enum
{
	INT,
	FLOAT,
	STRING,
	STRUCT,
	BOOL,
	LONG,
	DOUBLE,
	ENUM,
	ARRAY
} FieldType;

typedef struct
{
	char *name;
	int offset;
	FieldType type;
	struct StructScheme *childScheme;
} FieldInfo;

typedef struct StructScheme
{
	FieldInfo *fields;
	size_t size;
	int fieldCount;
	char *name;
} StructScheme;


typedef struct
{
	int size;
	FieldType elementType;
	void *data;
} SerializableArray;


SerializableArray CCereal_CreateArray(int size, FieldType elementType, void *data);

char *CCereal_Serialize(void *data, StructScheme *scheme);

void *CCereal_Deserialize(char *jsonString, StructScheme *scheme);
