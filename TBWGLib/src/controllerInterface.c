#include <stdlib.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/eventer.h>


struct EventerInformation* getEventerInformationsFromEventers(size_t eventerCount, struct Eventer* eventers)
{
	struct EventerInformation* r = malloc(sizeof(struct EventerInformation)*eventerCount);
	for(size_t i = 0 ; i < eventerCount ; i+=1) {
		r[i].eventerCode = eventers[i].eventerCode;
		getName(r[i].name, eventers[i].name);
		r[i].energyValueType = eventers[i].energyValueType;
		r[i].minEnergy = eventers[i].baseEnergy;
	}
	
}




void defaultControllerObserve(struct ControllerInterface*, struct ObservingInformation obsInfo)
{
}

void defaultControllerWorldEventObserve(struct ControllerInterface*, struct WorldEventInformation obsInfo)
{
}


struct TurnPlay defaultControllerChooseEventer(struct ControllerInterface*, id_number chooserId, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer** eventers, struct EventerUses restUses)
{
	struct EventerRequiredInformations reqs = {getiVector(0,0), getiVector(0,0), getfVector(1.0f, 0.0f), getiVector(0,0), getiVector(1,1)};
	struct TurnPlay result = {0, reqs, TURNPLAY_END_TURN};
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
	printf("\n\n\nInformations as SelfID:%u[%uHP, %uE, %uSE]\n", obsInfo.selfid, obsInfo.hp.value, obsInfo.e.value, obsInfo.se.value);
	printf("You're at (%i,%i)!\n",obsInfo.position.x, obsInfo.position.y);
	for(unsigned int i = 0 ; i < obsInfo.characterCount ; i += 1) {
		struct CharacterInformation chr = obsInfo.charInfos[i];
		printf("\tCharacter %u at (%i,%i) [%u/%uHP]!\n", chr.characterCode, chr.position.x, chr.position.y, chr.hp.value, chr.hp.max);
	}
}


void stdioControllerWorlEventObserve(struct ControllerInterface* interface, struct WorldEventInformation weInfo)
{
	printf("\n\n\nWorld Event Information as SelfID:%u\n",weInfo.selfid);
	printf("\t[%s] in (%i,%i)\n",weInfo.eventName, weInfo.position.x, weInfo.position.y);
}




struct TurnPlay stdioControllerChooseEventer(struct ControllerInterface*, id_number chooserId, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer** eventers, struct EventerUses restUses)
{
	struct TurnPlay result;
	result.specs = 0;
	printf("--Eventer Choosing for %u--\n", chooserId);
	printf("Remaining Uses:\n");

	//char* useCases[5] = {"Classic", "Movement", "Fast Combat", "Fast Magic", "Thought Magic"};
	char* useCases[5] = {"C","FC","M","FM","TM"};
	for(unsigned int i = 0 ; i<5 ; i += 1) {
		printf("\t%s:%i",useCases[i],((int*)&restUses)[i]);
	}
	printf("\n");

	printf("e:End turn\n");
	for(unsigned int i = 0 ; i < eventerCount ; i += 1) {
		printf("%u:Eventer %u [%s]",i,eventers[i]->eventerCode, eventers[i]->name);
		if (eventers[i]->eventer_type & allowedEventerTypes == 0) printf(" (not allowed)");
		printf(" with id %u\t",eventers[i]->ID);

		
		for(unsigned int j = 0 ; j<5 ; j += 1) {
			printf("%s:%i ", useCases[j], ((int*)(&(eventers[i]->costs)))[j] );
		}
		printf("\n");
	}

	char choice[8];
	printf("choice:");scanf("%s",choice);
	if (choice[0] == 'e') {
		result.specs = TURNPLAY_END_TURN;
		goto choosingFinished;
	}

	result.eventer_th = atoi(choice);
	printf("EVENT:%u\n",result.eventer_th);

	if (eventers[result.eventer_th]->required_informations & EVENTER_REQUIRED_INFORMATION_POSITION) {
		printf("x:");scanf("%u",&(result.requiredInformations.position.x));
		printf("y:");scanf("%u",&(result.requiredInformations.position.y));	
	} /*and*/ if (eventers[result.eventer_th]->required_informations & EVENTER_REQUIRED_INFORMATION_DIRECTION) {
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