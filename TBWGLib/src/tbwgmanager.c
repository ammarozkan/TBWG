#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <stdlib.h> // malloc

struct tbwgdata* data;

struct tbwgdata
tbwgInit()
{
	struct tbwgdata result;
	result.queue = createQueue();
	result.world = createDefaultWorld();
	return result;
}

void
tbwgUse(struct tbwgdata* udata)
{
	data = udata;
}

struct Dimension* tbwgFindDimensionByPos(unsigned int pos)
{
	unsigned int i = 0;
	struct Dimension* dimResult = NULL;
	ITERATE(data->world.dimensionList, dimension) {
		if (i == pos) {
			dimResult = ((struct DimensionListElement*)dimension)->dimension;
			break;
		}
		else i+=1;
	}
	return dimResult;
}

struct Dimension* tbwgFindDimensionByID(id_number ID)
{
	struct Dimension* dimResult = NULL;
	ITERATE(data->world.dimensionList, dimension) {
		if (((struct DimensionListElement*)dimension)->dimension->ID == ID) {
			dimResult = ((struct DimensionListElement*)dimension)->dimension;
			break;
		}
	}
	return dimResult;
}

struct Dimension* tbwgGetFirstDimension()
{
	return ((struct DimensionListElement*)(data->world.dimensionList.firstelement))->dimension;
}

TBWGType* tbwgFindBeingByPosition(unsigned int dimensionID)
{

	struct Dimension* dimension;
	ITERATE(data->world.dimensionList, dimension) {
		struct List characterList = ((struct DimensionListElement*)dimension)->dimension->characterList;
		ITERATE(characterList,charListElm_pure) {
			struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
			// base queue to data.queue
			
		}
	}
}

void tbwgReorder()
{
	ITERATE(data->world.dimensionList, dimension) {
		struct List characterList = ((struct DimensionListElement*)dimension)->dimension->characterList;
		ITERATE(characterList,charListElm_pure) {
			struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
			// base queue to data.queue
			mergeBaseQueue(&(data->queue), &(charListElm->character->baseQueue));
		}
	}
}

void tbwgMakeObserveAllCharacters()
{
	ITERATE_ALL_CHARACTERS_IN_WORLD((data->world), charlistelm, dimension) {
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;
		struct ObservingInformation obsInfo = Observe(chr, &(data->world));
		chr->controllerInterface->observer(chr->controllerInterface, obsInfo);
	}
}



void tbwgCharacterTurn(struct QueueCharacterTurn* turn)
{
	struct ControllerInterface* interface = turn->character->controllerInterface;
	turn->whenInvoked(turn);
	addEventerUses(&(turn->character->eventerSpendings), turn->gainingUses);

	struct TurnPlay choose;
	while( 1 ) {
		tbwgMakeObserveAllCharacters();
		struct Character* character = turn->character;

		choose = interface->chooseEventer(interface, turn->allowedEventerTypes, character->eventerCount, turn->character->eventers,
			character->eventerSpendings);
		if (choose.specs & TURNPLAY_END_TURN) return;

		struct Eventer* eventer = character->eventers + choose.eventer_th;

		if (checkRequiredEventers(character->eventerSpendings, eventer->costs))
			eventer->executer((void*)eventer, &(data->world), turn->character, choose.requiredInformations, NULL);
	}
}

void tbwgTurn()
{
	if (queueIsEmpty(&(data->queue))) {
		struct QueueReorderTurn turn; turn.header.type = QUEUE_REORDER;

		queueAddTurn(&(data->queue), (struct QueueElementHeader*)&turn);
		return;
	}

	struct QueueElementHeader* queueElement = queuePop(&(data->queue));

	if(queueElement->type == QUEUE_CHARACTER) tbwgCharacterTurn((struct QueueCharacterTurn*)queueElement);
	else if(queueElement->type == QUEUE_REORDER) tbwgReorder(data);

	free(queueElement);
}



int tbwgAddCharacter(struct Character* character)
{
	return addCharacterToWorld(&(data->world), character);
}

void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event)
{
	ITERATE(dim->characterList, characterElm) {
		struct Character* chr = ((struct CharacterListElement*)characterElm)->character;
		struct WorldEventInformation inf = ObserveWorldEventInformation(chr, &event);
		if (inf.eventName[0] != '\0') chr->controllerInterface->receiveWorldEvent(chr->controllerInterface, inf);
	}
}
