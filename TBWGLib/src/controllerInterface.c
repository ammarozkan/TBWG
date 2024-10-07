#include <stdlib.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/eventer.h>

void defaultControllerObserve(struct ControllerInterface*, struct ObservingInformation obsInfo)
{
}


struct EventerChoose defaultControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct EventerChoose result = {0, NULL};
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


struct EventerChoose stdioControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct EventerChoose result;
	printf("--Eventer Choosing--\n");
	for(unsigned int i = 0 ; i < eventerCount ; i += 1) {
		printf("%u:Eventer %u",i,eventers[i].eventerCode);
		if (eventers[i].eventer_type & allowedEventerTypes == 0) printf(" (not allowed)");
		printf(" with id %u\n",eventers[i].ID);
	}
	printf("choice:");scanf("%u",&result.eventer_th);

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