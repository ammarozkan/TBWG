#ifndef TBWG_ESSENTIALS_H
#define TBWG_ESSENTIALS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define TBWG_TYPE_UNKNOWN 0x00
#define TBWG_TYPE_CHARACTER 0x05
#define TBWG_TYPE_ENTITY 0x06
#define TBWG_TYPE_AREA 0x07
#define TBWG_TYPE_DIMENSION 0x08
#define TBWG_TYPE_TOOL 0x09

#define GET_SIZED_STRUCT(structName) malloc(sizeof(struct structName))

struct iLimitedValue {
	int value, max;
};

typedef uint32_t id_number;
typedef uint32_t digits32;

typedef uint8_t TBWGType;

typedef struct iLimitedValue iValue;
typedef char Name[32];

void getName(Name memPlace, char*);

id_number getID();
void tbwgmemcpy(void* dest, void* source, long unsigned int size);

#endif