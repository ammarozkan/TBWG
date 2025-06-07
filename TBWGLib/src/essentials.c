#include <stdlib.h> // malloc, free
#include <TBWG/essentials.h>

void tbwgmemcpy(void* _dest, void* _source, long unsigned int size)
{
	uint8_t* dest = (uint8_t*)_dest;
	uint8_t* source = (uint8_t*)_source;

	for(long unsigned int i = 0 ; i < size ; i += 1) {
		*(dest+i) = *(source+i);
	}
}

id_number getID()
{
	static id_number id = 5;
	return id++;
}

size_t tbwgstrlen(char* str)
{
	size_t i = 0;
	for(;str[i]!='\0';i+=1);
	return i;
}


void getName(Name memPlace, char* n)
{
	tbwgmemcpy(memPlace, n, tbwgstrlen(n/*+1*/)+1);
}

int addToiValue(iValue* v, int val)
{
	v->value += val;
	if (v->value > v->max) {
		int r = v->max - v->value;
		v->value -= r;
		return r;
	} return 0;
}