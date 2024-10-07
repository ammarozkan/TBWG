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

void tbwgCharacterTurn(struct QueueCharacterTurn* turn)
{
	struct ControllerInterface* interface = turn->character->controllerInterface;
	struct EventerChoose choose = interface->chooseEventer(interface, turn->allowedEventerTypes, turn->character->eventerCount, turn->character->eventers);

	struct Character* character = turn->character;

	struct Eventer* eventer = character->eventers + choose.eventer_th;

	eventer->executer((void*)eventer, &(data->world), turn->character, choose.target, NULL);

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
	struct Dimension* dimension = character->dimension;
	if(dimension == NULL) return 0;

	struct List* characterList = &(dimension->characterList);
	struct CharacterListElement charListElm = {.character = character};

	addElement(characterList, (void*)&charListElm, sizeof(struct CharacterListElement));
	return 1;
}