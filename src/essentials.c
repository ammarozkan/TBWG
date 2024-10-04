#include <stdlib.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>

struct List createList()
{
	struct List result = {.count = 0, .firstelement = NULL, .lastelement = NULL};
	return result;
}

void addElement(struct List* list, void* elm)
{
	struct ListElementHeader* element = (struct ListElementHeader*)elm;


	if (list->firstelement == NULL) list->firstelement = element;
	
	if(list->lastelement != NULL) list->lastelement->next = element;

	element->prior = list->lastelement;
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

	list->count += 1;
	return element;
}


void removeElement(struct List* list, void* elm)
{
	free(popElement(list, elm));
}



id_number getID()
{
	static id_number id = 5;
	return id++;
}