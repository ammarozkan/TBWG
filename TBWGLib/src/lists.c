#include <TBWG/essentials.h>
#include <TBWG/lists.h>

struct List createList()
{
	struct List result = {.count = 0, .firstelement = NULL, .lastelement = NULL};
	return result;
}


void freeListHeaders(struct List l)
{
	void** ptrs = tbwgmalloc(sizeof(void*)*(l.count));

	unsigned int i = 0;
	ITERATE(l, elm) {
		ptrs[i] = elm;
		i+=1;
	}

	for(unsigned int j = 0 ; j < i ; j += 1) {
		free(ptrs[j]);
	}free(ptrs);
}


void freeListsHeaders(struct List* ls, unsigned int listCount)
{
	for(unsigned int i = 0 ; i < listCount ; i += 1) {
		freeListHeaders(ls[i]);
	}
}

void addElement(struct List* list, void* elm, size_t size)
{
	struct ListElementHeader* element = malloc(size);
	tbwgmemcpy(element, elm, size);


	if (list->firstelement == NULL) list->firstelement = element;
	
	if(list->lastelement != NULL) list->lastelement->next = element;

	element->prior = list->lastelement;
	element->next = NULL;
	list->lastelement = element;
	list->count += 1;
}


void* popElement(struct List* list, void* elm)
{
	struct ListElementHeader* element = (struct ListElementHeader*)elm;

	if(element->next == NULL) {
		list->lastelement = element->prior;
	} else {
		element->next->prior = element->prior;
	}

	if(element->prior == NULL) {
		list->firstelement = element->next;
	} else {
		element->prior->next = element->next;
	}

	list->count -= 1;
	return element;
}


void destroyElement(struct List* list, void* elm)
{
	free(popElement(list, elm));
}

void decolonizeList(struct List* list)
{
	void** elms = malloc(sizeof(void*)*list->count);

	unsigned int i = 0 ;
	ITERATE((*list), elm) {
		elms[i++] = elm;
	}

	for(unsigned int j = 0 ; j < i ; j += 1) {
		destroyElement(list, elms[j]);
	}

	free(elms);
}

int 
listIsEmpty(struct List* list)
{
	return list->firstelement == NULL || list->lastelement == NULL;
}


unsigned int getElementCountOfList(struct List list)
{
	return list.count;
}

struct List subtractList(struct List a, struct List b) // result = a - b // negative doesnt exist
{
	struct List result = createList();

	ITERATE_STD(a, a_elm, struct PtrElementHeader) {
		ITERATE_STD(b, b_elm, struct PtrElementHeader) {
			if (b_elm->ptr == a_elm->ptr) goto conloop;
		}
		struct PtrElementHeader hd = {.ptr = a_elm->ptr};
		addElement(&result, (void*)&hd, sizeof(hd));
conloop: continue;
	}

	return result;
}