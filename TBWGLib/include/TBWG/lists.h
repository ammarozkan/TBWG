#ifndef TBWG_LISTS_H
#define TBWG_LISTS_H


struct ListElementHeader{
	struct ListElementHeader* prior;
	struct ListElementHeader* next;
};

struct PtrElementHeader {
	struct ListElementHeader header;
	void* ptr;
};

struct List {
	struct ListElementHeader* firstelement;
	struct ListElementHeader* lastelement;
	unsigned int count;
};

#define ITERATE(list, asthis) for (struct ListElementHeader* asthis = (struct ListElementHeader*)list.firstelement; asthis != NULL; asthis = asthis->next)
#define ITERATE_(list, asthis, type, next) for (type* asthis = (type*)list.firstelement; asthis != NULL; asthis = asthis next)
#define ITERATE_STD(list, asthis, type) for (type* asthis = (type*)list.firstelement; asthis != NULL; asthis = (type*)asthis->header.next)

struct List createList();
void freeListHeaders(struct List);
void freeListsHeaders(struct List*, unsigned int listCount);

void addElement(struct List* list, void* element, long unsigned int size);
void destroyElement(struct List* list, void* element);
void* popElement(struct List* list, void* element);
void decolonizeList(struct List* list);
int listIsEmpty(struct List* list);

struct List subtractList(struct List a, struct List b); // result = a - b // negative doesnt exist

#endif /*TBWG_LISTS_H*/