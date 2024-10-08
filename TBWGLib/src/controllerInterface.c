#include <stdlib.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/eventer.h>

void defaultControllerObserve(struct ControllerInterface*, struct ObservingInformation obsInfo)
{
}


struct TurnPlay defaultControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct TurnPlay result = {0, NULL};
	return result;
}


struct ControllerInterface* getDefaultControllerInterface()
{
	struct ControllerInterface* interface = malloc(sizeof(struct ControllerInterface));
	interface->observer = defaultControllerObserve;
	interface->chooseEventer = defaultControllerChooseEventer;

	return interface;
}




#include <stdio.h>


void stdioControllerObserve(struct ControllerInterface*, struct ObservingInformation obsInfo)
{
}


struct TurnPlay stdioControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct TurnPlay result;
	printf("--Eventer Choosing--\n");
	printf("e:End turn\n");
	for(unsigned int i = 0 ; i < eventerCount ; i += 1) {
		printf("%u:Eventer %u",i,eventers[i].eventerCode);
		if (eventers[i].eventer_type & allowedEventerTypes == 0) printf(" (not allowed)");
		printf(" with id %u\n",eventers[i].ID);
	}

	char choice[8];
	printf("choice:");scanf("%s",choice);
	result.eventer_th = atoi(choice);
	printf("EVENT:%u\n",result.eventer_th);

	if (eventers[result.eventer_th].target_type = TARGET_ONE) {
		struct TargetPosition* target = malloc(sizeof(struct TargetPosition));
		printf("x:");scanf("%u",&target->x);
		printf("y:");scanf("%u",&target->y);

		result.target = (void*)target;
	}
	return result;
}


struct ControllerInterface* getstdioControllerInterface()
{
	struct ControllerInterface* interface = malloc(sizeof(struct ControllerInterface));
	interface->observer = stdioControllerObserve;
	interface->chooseEventer = stdioControllerChooseEventer;

	return interface;
}