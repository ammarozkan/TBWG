#include <stdio.h>
#include <stdlib.h>

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/maths.h>
#include <TBWG/system/comboFunctions.h>
#include <TBWG/randomness.h>

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

int example1(void*ptr,struct ComboPositionlessInstructors* instructors, int a)
{
	return -1;
}

int example2(void*ptr,struct ComboPositionlessInstructors* instructors, int a)
{
	int* overload = (int*)ptr;
	if(*overload == 1) {
		instructors->importanceLevel = 2;
	}
	return 1000;
}

int main(int argc, char*argv[])
{
	printf("Random percentage test\n");
	for (unsigned int i = 0 ; i < 20 ; i += 1) {
		printf("T:%i\n",tbwgRandomPercentageIncrease(100, -30, 30));
	}
	printf("Testing combo functions.\n");
	struct ComboFunction cf_i = tbwgGetComboFunction((ComboFunctionExecuterSkeleton)tbwgComboFunctionExecuter_i);

	int resCFI = ((tbwgComboFunctionExecuter_iType)(cf_i.executer))(&cf_i, 5);
	if (resCFI != 5) return -90;
	printf("result of cf_i is %i\n",resCFI);

	int overload = 0;
	printf("adding a function to combo function.\n");
	id_number idOfNewlyAddedOne = tbwgComboFunctionAddSoloFunction(&cf_i, (SoloFunctionSkeleton)example1, NULL);
	id_number idOfNewlyAddedOne2 = tbwgComboFunctionAddSoloFunction(&cf_i, (SoloFunctionSkeleton)example2, &overload);
	printf("added function:%u %u\n",idOfNewlyAddedOne, idOfNewlyAddedOne2);

	printf("Executing.\n");
	resCFI = ((tbwgComboFunctionExecuter_iType)(cf_i.executer))(&cf_i, 5);
	printf("result of cf_i is %i\n",resCFI);
	if (resCFI != -1) return -91;
	printf("OVERLOADING!\n");
	overload = 1;
	resCFI = ((tbwgComboFunctionExecuter_iType)(cf_i.executer))(&cf_i, 5);
	printf("result of cf_i is %i\n",resCFI);
	if (resCFI != 1000) return -92;

	printf("removing the solo2 (%u)\n",idOfNewlyAddedOne2);
	tbwgComboFunctionRemoveSoloFunction(&cf_i, idOfNewlyAddedOne2);
	printf("removed! now trying it out.\n");
	resCFI = ((tbwgComboFunctionExecuter_iType)(cf_i.executer))(&cf_i, 5);
	printf("result of cf_i is %i\n",resCFI);
	if (resCFI != -1) return -92;
	printf("OK!\n");

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

	tbwgInit();

	printf("Reorder test!\n");
	tbwgReorder();

	printf("getID() test!\n");

	for(unsigned int i = 0 ; i < 12 ; i += 1) {
		printf("\t%u: ID = %u\n", i, getID());
	}

	printf("Character creation test!\n");

	struct Character* character1 = createDefaultCharacter(tbwgGetFirstDimension(), getiVector(1,0));
	chAddEffect(getTestingHitEffect(NULL), EFFECT_TRIGGER_TYPE_HIT, character1);

	struct Character* character2 = createDefaultCharacter(tbwgGetFirstDimension(), getiVector(0,0));
	struct Character* character3 = createDefaultCharacter(tbwgGetFirstDimension(), getiVector(0,1));

	printf("Adding character to tbwgmanager\n");

	tbwgAddCharacter(character1);
	tbwgAddCharacter(character2);
	tbwgAddCharacter(character3);

	printf("Reordering again!\n");

	tbwgReorder();

	printf("Printing queue\n");


	ITERATE(tbwgGetUsenData()->queue.queueElements, element) {
		struct QueueElementHeader* hed = (struct QueueElementHeader*)element;
		printf("Turn:%u ", hed->type);
		if(hed->type == QUEUE_CHARACTER) printf("Character:0x%x", ((struct QueueCharacterTurn*)hed)->character);

		printf("\n");
	}

	printf("Regening health for character1!\n");
	character1->hp.value = 5;
	printf("Health now:%i\n",character1->hp.value);
	chHealthRegener(character1, 20);
	printf("Health now:%i\n",character1->hp.value);

	for(unsigned int i = 0 ; i < 10 ; i += 1) {
		printf("tbwgTurn test!\n");

		tbwgTurn();

		printf("Printing queue again\n");

		ITERATE(tbwgGetUsenData()->queue.queueElements, element) {
			struct QueueElementHeader* hed = (struct QueueElementHeader*)element;
			printf("Turn:%u ", hed->type);
			if(hed->type == QUEUE_CHARACTER) printf("Character:0x%x", ((struct QueueCharacterTurn*)hed)->character);

			printf("\n");
		}
	}

	printf("Testing completed\n");

	return 0;
}
