
struct ListElementHeader{
	struct ListElementHeader* prior;
	struct ListElementHeader* next;
};

struct List {
	struct ListElementHeader* firstelement;
	struct ListElementHeader* lastelement;
	unsigned int count;
};

#define ITERATE(list, asthis) for (struct ListElementHeader* asthis = list.firstelement; asthis != NULL; asthis = asthis->next)
#define ITERATE_(list, asthis, type, next) for (type* asthis = (type*)list.firstelement; asthis != NULL; asthis = asthis next)

void addElement(struct List* list, struct ListElementHeader* element);
void removeElement(struct List* list, struct ListElementHeader* element);
struct List createList();