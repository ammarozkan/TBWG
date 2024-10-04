#include <stdio.h>
#include <stdlib.h>

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>

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

	printf("First Element:0x%x, LastElement:0x%x\n",list.firstelement, list.lastelement);

	ITERATE(list, element) {
		struct CustomListElement* elm = (struct CustomListElement*)element;
		printf("%s!\n",elm->name);
	}

	struct tbwgdata data = tbwgInit();

	printf("Reorder test!\n");
	tbwgReorder(&data);

	printf("getID() test!\n");

	for(unsigned int i = 0 ; i < 12 ; i += 1) {
		printf("\t%u: ID = %u\n", i, getID());
	}

	printf("Character creation test!\n");

	struct Character* character1 = createDefaultCharacter();

	printf("Adding character to tbwgmanager\n");

	tbwgAddCharacter(&data, character1);

	printf("Reordering again!\n");

	tbwgReorder(&data);

	printf("Testing completed\n");

	return 0;
}
