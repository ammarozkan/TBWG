#ifndef TBWG_ESSENTIALS_H
#define TBWG_ESSENTIALS_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t id_number;
typedef uint32_t digits32;

id_number getID();


#define TBWG_TYPE_CHARACTER 0x05
#define TBWG_TYPE_ENTITY 0x06

typedef uint8_t TBWGType;

void tbwgmemcpy(void* dest, void* source, long unsigned int size);

struct iLimitedValue {
	int value, max;
};

typedef struct iLimitedValue iValue;
typedef char Name[32];

void getName(Name memPlace, char*);

#endif