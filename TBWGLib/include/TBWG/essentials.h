#ifdef TBWG_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(func,txt) printf("DEBUG:%s %s\n",func,txt)
#define DEBUG_PRINTINT(func,txt,int) printf("DEBUG:%s %s ([%i])\n",func,txt,int)
#define DEBUG_PRINTUINT(func,txt,uint) printf("DEBUG:%s %s ([%u])\n",func,txt,uint)
#define DEBUG_RECEIVEDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode) printf("DEBUG:%s (%u)%u sized (%u)%u code pkg reaceived\n", func, targetsize, recvsize, targetpkgcode, pkgcode)
#define DEBUG_SENDDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode) printf("DEBUG:%s (%u)%u sized (%u)%u code pkg sent\n", func, targetsize, recvsize, targetpkgcode, pkgcode)
#else
#define DEBUG_PRINT(x,y)
#define DEBUG_PRINTINT(func,txt,int)
#define DEBUG_PRINTUINT(func,txt,uint)
#define DEBUG_RECEIVEDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode)
#define DEBUG_SENDDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode)
#endif /*TBWG_DEBUG*/

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
#define NEW(structName) GET_SIZED_STRUCT(structName)

struct iLimitedValue {
	int value, max;
};


#define TBWG_ID_UNKNOWN 0x00
#define TBWG_ID_NEW 0x01

typedef uint32_t id_number;
typedef uint32_t digits32;

typedef uint8_t TBWGType;

typedef struct iLimitedValue iValue;
typedef char Name[32];

void getName(Name memPlace, char*);

id_number getID();
void tbwgmemcpy(void* dest, void* source, long unsigned int size);
size_t tbwgstrlen(char* str);
int addToiValue(iValue* v, int val);
#define tbwgmalloc malloc

#endif