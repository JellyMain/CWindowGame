#include "metaData.h"
#include <stddef.h>
#include "../../Scripts/General/Headers/structs.h"

FieldInfo Vector2FloatFields[] = {
    { .name = "x", .offset = offsetof(Vector2Float, x), .type = FLOAT, .childScheme = NULL },
    { .name = "y", .offset = offsetof(Vector2Float, y), .type = FLOAT, .childScheme = NULL }
};

FieldInfo PlayerDataFields[] = {
    { .name = "randomNumber", .offset = offsetof(PlayerData, randomNumber), .type = FLOAT, .childScheme = NULL }
};

StructScheme Vector2FloatScheme = { .name = "Vector2Float", .fieldCount = 2, .fields = Vector2FloatFields, .size = sizeof(Vector2Float) };

StructScheme PlayerDataScheme = { .name = "PlayerData", .fieldCount = 1, .fields = PlayerDataFields, .size = sizeof(PlayerData) };

