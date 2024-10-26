#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <TBWG/effects.h>
#include <TBWG/areas.h>
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

struct World* tbwgGetWorld()
{
	return &(data->world);
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

TBWGType* tbwgFindBeingByPosition(struct Dimension* dim, int x, int y)
{

	struct List characterList = dim->characterList;
	struct Character* chr = dimensionGetCharacterByPosition(dim, x, y);
	// ACTUALLY THESE INFORMATIONS ARE READY IN DIMENSION FUNCTIONS, NO NEED THEM HERE.
	return NULL;
}

void tbwgTriggerEffects(unsigned int effectType, void* relativeInformation)
{
	ITERATE_ALL_CHARACTERS_IN_WORLD((data->world), charlistelm, dimension) {
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;
		chTriggerEffect(chr, &(data->world), effectType, relativeInformation);
	}
}

void tbwgTriggerAreaWhileInside(struct Dimension* dim, struct Area* area) 
{
	struct List chs = dimensionGetInAreaCharacters(dim, area->a, area->b);
	ITERATE(chs, char_elm) {
		struct Character* ch = ((struct CharacterListElement*)char_elm)->character;
		area->whileInside(area, (struct Being*)ch);
	}
	decolonizeList(&chs);
}

void tbwgTriggerAreaWhileInsides()
{
	ITERATE(data->world.dimensionList, dimension) {
		struct Dimension* dim = ((struct DimensionListElement*)dimension)->dimension;
		struct List areaList = dim->areaList;
		ITERATE(areaList, area_elm) {
			struct Area* area = ((struct AreaListElement*)area_elm)->area;
			tbwgTriggerAreaWhileInside(dim, area);
		}
	}
}

void tbwgReorder()
{
	tbwgTriggerEffects(EFFECT_TRIGGER_TYPE_CLOCK, NULL);
	tbwgTriggerAreaWhileInsides();

	ITERATE(data->world.dimensionList, dimension) {
		struct List characterList = ((struct DimensionListElement*)dimension)->dimension->characterList;
		ITERATE(characterList,charListElm_pure) {
			struct BeingListElement* charListElm = (struct BeingListElement*)charListElm_pure;
			// base queue to data.queue
			mergeBaseQueue(&(data->queue), &(charListElm->being->baseQueue));
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

		choose = interface->chooseEventer(interface, character->b.ID, turn->allowedEventerTypes, character->eventerCount, turn->character->eventers,
			character->eventerSpendings);
		if (choose.specs & TURNPLAY_END_TURN) return;

		struct Eventer* eventer = character->eventers[choose.eventer_th];

		if (eventer->baseEnergy > character->e.value || eventer->baseSpellEnergy > character->se.value) {
			continue; // failed use
		}

		if (!useEventerRequirements(&(character->eventerSpendings), eventer->costs)) continue;
		

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
	return worldAddCharacter(&(data->world), character);
}

int tbwgAddArea(struct Dimension* dim, struct Area* area)
{
	return dimensionAddArea(dim, area);
}

void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event)
{
	ITERATE(dim->characterList, characterElm) {
		struct Character* chr = ((struct CharacterListElement*)characterElm)->character;
		struct WorldEventInformation inf = ObserveWorldEventInformation(chr, &event);
		if (inf.eventName[0] != '\0') chr->controllerInterface->receiveWorldEvent(chr->controllerInterface, inf);
	}
}





void tbwgMoveBeing(struct Being* b, iVector pc)
{
	tbwgPutBeing(b, getiVector(b->position.x + pc.x, b->position.y + pc.y));
}

void tbwgPutBeing(struct Being* b, iVector position)
{
	struct List pre_areas = dimensionGetAreasOfPosition(b->dimension, b->position);
	struct List aft_areas = dimensionGetAreasOfPosition(b->dimension, position);


	struct List old_areas = subtractList(pre_areas, aft_areas);
	struct List new_areas = subtractList(aft_areas, pre_areas);


	decolonizeList(&pre_areas);
	decolonizeList(&aft_areas);


	ITERATE(old_areas, area_elm) {
		struct Area* area = ((struct AreaListElement*)area_elm)->area;
		area->whenExited(area, b);
	}

	ITERATE(new_areas, area_elm) {
		struct Area* area = ((struct AreaListElement*)area_elm)->area;
		area->whenEntered(area, b);
	}

	decolonizeList(&new_areas);
	decolonizeList(&old_areas);

	b->position = position;
}