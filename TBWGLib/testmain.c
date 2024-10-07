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

struct CustomListElement getELM(char* name)
{
	struct CustomListElement result = {.name = name};
	return result;
}

int main(int argc, char*argv[])
{
	struct List list = createList();
	struct CustomListElement cle = getELM("test1");
	addElement(&list, &cle, sizeof(struct CustomListElement));
	cle = getELM("test2");
	addElement(&list, &cle, sizeof(struct CustomListElement));
	addElement(&list, &cle, sizeof(struct CustomListElement));
	cle = getELM("test3");
	addElement(&list, &cle, sizeof(struct CustomListElement));

	printf("First Element:0x%x, LastElement:0x%x\n",list.firstelement, list.lastelement);

	ITERATE(list, element) {
		struct CustomListElement* elm = (struct CustomListElement*)element;
		printf("%s!\n",elm->name);
	}

	struct tbwgdata data = tbwgInit();
	tbwgUse(&data);

	printf("Reorder test!\n");
	tbwgReorder();

	printf("getID() test!\n");

	for(unsigned int i = 0 ; i < 12 ; i += 1) {
		printf("\t%u: ID = %u\n", i, getID());
	}

	printf("Character creation test!\n");

	struct Character* character1 = createDefaultCharacter(tbwgGetFirstDimension(&data));
	struct Character* character2 = createDefaultCharacter(tbwgGetFirstDimension(&data));
	struct Character* character3 = createDefaultCharacter(tbwgGetFirstDimension(&data));

	printf("Adding character to tbwgmanager\n");

	tbwgAddCharacter(character1);
	tbwgAddCharacter(character2);
	tbwgAddCharacter(character3);

	printf("Reordering again!\n");

	tbwgReorder();

	printf("Printing queue\n");

	ITERATE(data.queue.queueElements, element) {
		struct QueueElementHeader* hed = (struct QueueElementHeader*)element;
		printf("Turn:%u ", hed->type);
		if(hed->type == QUEUE_CHARACTER) printf("Character:0x%x", ((struct QueueCharacterTurn*)hed)->character);

		printf("\n");
	}

	for(unsigned int i = 0 ; i < 10 ; i += 1) {
		printf("tbwgTurn test!\n");

		tbwgTurn();

		printf("Printing queue again\n");

		ITERATE(data.queue.queueElements, element) {
			struct QueueElementHeader* hed = (struct QueueElementHeader*)element;
			printf("Turn:%u ", hed->type);
			if(hed->type == QUEUE_CHARACTER) printf("Character:0x%x", ((struct QueueCharacterTurn*)hed)->character);

			printf("\n");
		}
	}

	printf("Testing completed\n");

	return 0;
}
