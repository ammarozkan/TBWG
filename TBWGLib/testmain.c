#include <stdio.h>
#include <stdlib.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>

struct CustomListElement {
	struct ListElementHeader header;
	char* name;
};

struct ListElementHeader* getELM(char* name)
{
	struct CustomListElement* result = malloc(sizeof(struct CustomListElement));
	result->name = name;
	return (struct ListElementHeader*)result;
}

int main(int argc, char*argv[])
{
	struct List list = createList();
	addElement(&list, getELM("test1"));
	addElement(&list, getELM("test2"));
	addElement(&list, getELM("test3"));

	ITERATE(list, element) {
		struct CustomListElement* elm = (struct CustomListElement*)element;
		printf("%s!\n",elm->name);
	}

	return 0;
}
