#include <stdlib.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/eventer.h>

void defaultControllerObserve(struct ControllerInterface*, struct ObservingInformation obsInfo)
{
}

void defaultControllerWorldEventObserve(struct ControllerInterface*, struct WorldEventInformation obsInfo)
{
}


struct TurnPlay defaultControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct EventerRequiredInformations reqs = {getiVector(0,0), getfVector(1.0f, 0.0f), getiVector(0,0), getiVector(1,1)};
	struct TurnPlay result = {0, reqs};
	return result;
}




struct ControllerInterface* getDefaultControllerInterface()
{
	struct ControllerInterface* interface = malloc(sizeof(struct ControllerInterface));
	interface->observer = defaultControllerObserve;
	interface->chooseEventer = defaultControllerChooseEventer;
	interface->receiveWorldEvent = defaultControllerWorldEventObserve;

	return interface;
}




#include <stdio.h>


void stdioControllerObserve(struct ControllerInterface* interface, struct ObservingInformation obsInfo)
{
	printf("\n\n\nInformations as SelfID:%u\n",obsInfo.selfid);
	printf("You're at (%i,%i)!\n",obsInfo.position.x, obsInfo.position.y);
	for(unsigned int i = 0 ; i < obsInfo.characterCount ; i += 1) {
		struct CharacterInformation chr = obsInfo.charInfos[i];
		printf("\tCharacter %u at (%i,%i)!\n", chr.characterCode, chr.position.x, chr.position.y);
	}
}


void stdioControllerWorlEventObserve(struct ControllerInterface* interface, struct WorldEventInformation weInfo)
{
	printf("\n\n\nWorld Event Information as SelfID:%u\n",weInfo.selfid);
	printf("\t[%s] in (%i,%i)\n",weInfo.eventName, weInfo.position.x, weInfo.position.y);
}


struct TurnPlay stdioControllerChooseEventer(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers)
{
	struct TurnPlay result;
	result.specs = 0;
	printf("--Eventer Choosing--\n");
	printf("e:End turn\n");
	for(unsigned int i = 0 ; i < eventerCount ; i += 1) {
		printf("%u:Eventer %u [%s]",i,eventers[i].eventerCode, eventers[i].name);
		if (eventers[i].eventer_type & allowedEventerTypes == 0) printf(" (not allowed)");
		printf(" with id %u\n",eventers[i].ID);
	}

	char choice[8];
	printf("choice:");scanf("%s",choice);
	if (choice[0] == 'e') {
		result.specs = TURNPLAY_END_TURN;
		goto choosingFinished;
	}

	result.eventer_th = atoi(choice);
	printf("EVENT:%u\n",result.eventer_th);

	if (eventers[result.eventer_th].required_informations & EVENTER_REQUIRED_INFORMATION_POSITION) {
		printf("x:");scanf("%u",&(result.requiredInformations.position.x));
		printf("y:");scanf("%u",&(result.requiredInformations.position.y));	
	} /*and*/ if (eventers[result.eventer_th].required_informations & EVENTER_REQUIRED_INFORMATION_DIRECTION) {
		printf("dirx:");scanf("%f",&(result.requiredInformations.direction.x));
		printf("diry:");scanf("%f",&(result.requiredInformations.direction.y));	
	}

choosingFinished:
	return result;
}


struct ControllerInterface* getstdioControllerInterface()
{
	struct ControllerInterface* interface = malloc(sizeof(struct ControllerInterface));
	interface->observer = stdioControllerObserve;
	interface->chooseEventer = stdioControllerChooseEventer;
	interface->receiveWorldEvent = stdioControllerWorlEventObserve;

	return interface;
}