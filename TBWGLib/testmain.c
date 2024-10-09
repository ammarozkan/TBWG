#include <stdio.h>
#include <stdlib.h>

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/maths.h>

struct CustomListElement {
	struct ListElementHeader header;
	char* name;
};

struct CustomListElement getELM(char* name)
{
	struct CustomListElement result = {.name = name};
	return result;
}

void getSpecTest(iVector tp, fVector dir)
{
	printf("Looking to (%i,%i) from (%i,%i) with direction (%f,%f):", tp.x, tp.y, 0,0, dir.x, dir.y);
	printf("%i\n", isInVisionArea(dir, 2.8f, getiVector(0,0), tp) );
}

void getTest(iVector tp)
{
	printf("[UP] "); getSpecTest(tp, getfVector(0.0f,1.0f) );
	printf("[RIGHT] "); getSpecTest(tp, getfVector(1.0f,0.0f) );
}

int main(int argc, char*argv[])
{
	printf("Testing look dir math 3.14f look angle.\n\n");
	getTest(getiVector(10,0));
	getTest(getiVector(28,2));
	getTest(getiVector(-10,0));
	printf("\n\n");

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

	struct Character* character1 = createDefaultCharacter(tbwgGetFirstDimension(&data), getiVector(1,0));
	struct Character* character2 = createDefaultCharacter(tbwgGetFirstDimension(&data), getiVector(0,0));
	struct Character* character3 = createDefaultCharacter(tbwgGetFirstDimension(&data), getiVector(0,1));

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
