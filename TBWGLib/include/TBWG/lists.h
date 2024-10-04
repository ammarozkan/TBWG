#ifndef TBWG_LISTS_H
#define TBWG_LISTS_H


struct ListElementHeader{
	struct ListElementHeader* prior;
	struct ListElementHeader* next;
};

struct List {
	struct ListElementHeader* firstelement;
	struct ListElementHeader* lastelement;
	unsigned int count;
};

#define ITERATE(list, asthis) for (struct ListElementHeader* asthis = (struct ListElementHeader*)list.firstelement; asthis != NULL; asthis = asthis->next)
#define ITERATE_(list, asthis, type, next) for (type* asthis = (type*)list.firstelement; asthis != NULL; asthis = asthis next)

void addElement(struct List* list, void* element);
void removeElement(struct List* list, void* element);
void* popElement(struct List* list, void* element);
struct List createList();

#endif /*TBWG_LISTS_H*/