#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/entity.h>
#include <TBWG/eventer.h>
#include <TBWG/effects.h>
#include <TBWG/areas.h>
#include <stdlib.h> // malloc

#ifdef TBWG_DEBUG
#include <stdio.h>
#endif

struct tbwgdata* data;

struct tbwgdata*
tbwgInit()
{
	struct tbwgdata* result = NEW(tbwgdata);
	result->queue = createQueue();
	result->world = createDefaultWorld();
	data = result;
	return result;
}

void
tbwgUse(struct tbwgdata* udata)
{
	data = udata;
}

struct tbwgdata*
tbwgGetUsenData()
{
	return data;
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
	DEBUG_PRINT("tbwgTriggerAreaWhileInsides","Starting to iterate dimensions.");
	ITERATE(data->world.dimensionList, dimension) {
		struct Dimension* dim = ((struct DimensionListElement*)dimension)->dimension;
		struct List areaList = dim->areaList;
		DEBUG_PRINT("tbwgTriggerAreaWhileInsides","Starting to iterate areas.");
		ITERATE(areaList, area_elm) {
			DEBUG_PRINT("tbwgTriggerAreaWhileInsides","Getting area from element.");
			DEBUG_PRINTUINT("tbwgTriggerAreaWhileInsides", "area_elm", area_elm);
			struct Area* area = ((struct AreaListElement*)area_elm)->area;
			DEBUG_PRINT("tbwgTriggerAreaWhileInsides","Calling trigger area while inside.");
			tbwgTriggerAreaWhileInside(dim, area);
		}
	}
}

void tbwgReorder()
{
	DEBUG_PRINT("tbwgReorder","Triggerring clock effects.");
	tbwgTriggerEffects(EFFECT_TRIGGER_TYPE_CLOCK, NULL);
	DEBUG_PRINT("tbwgReorder","Triggering area while insides.");
	tbwgTriggerAreaWhileInsides();
	DEBUG_PRINT("tbwgReorder","Starting to iterate dimension list.");
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
	DEBUG_PRINT("tbwgCharacterTurn","Eventer control ->");
	ITERATE_ALL_CHARACTERS_IN_WORLD((data->world), charlistelm, dimension) {
		DEBUG_PRINT("tbwgMakeObserveAllCharacters","Iterating character ->");
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;
		DEBUG_PRINT("tbwgMakeObserveAllCharacters","Observe information fetch ->");
		struct ObservingInformation obsInfo = Observe(chr, &(data->world));
		DEBUG_PRINT("tbwgMakeObserveAllCharacters","Character observe ->");
		chr->controllerInterface->observer(chr->controllerInterface, obsInfo);
	}
}



void tbwgCharacterTurn(struct QueueCharacterTurn* turn)
{
	DEBUG_PRINT("tbwgCharacterTurn","Character turn!");
	struct Character* character = turn->character;
	struct ControllerInterface* interface = character->controllerInterface;
	DEBUG_PRINT("tbwgCharacterTurn","Invoked will be called!");
	turn->whenInvoked(turn);
	DEBUG_PRINT("tbwgCharacterTurn","Invoked called!");
	//addEventerUses(&(turn->character->eventerSpendings), turn->gainingUses);
	updateEventerUses(&(character->eventerSpendings), turn->gainingUses);
	DEBUG_PRINT("tbwgCharacterTurn","Eventer uses updated.");

	DEBUG_PRINT("tbwgCharacterTurn","Dead state control ->");
	//if (turn->character->state & STATE_DEAD) return;
	DEBUG_PRINT("tbwgCharacterTurn","Hp value control ->");
	if (character->hp.value < 0) {
		DEBUG_PRINT("tbwgCharacterTurn","State setting ->");
		character->state = character->state | STATE_FAINTED | STATE_ONGROUND;
	}
	else if (character->hp.value > 0) character->state = character->state & (~STATE_FAINTED);

	struct TurnPlay choose;

	for(unsigned int i = 0 ; i < character->eventerCount ; i += 1)
		character->eventers[i]->isChoosed = 0;

	DEBUG_PRINT("tbwgCharacterTurn","Eventer while! ->");

	while( 1 ) {
		
		chUpdateStats(character);

		DEBUG_PRINT("tbwgCharacterTurn","Make observe ->");
		tbwgMakeObserveAllCharacters();

		DEBUG_PRINT("tbwgCharacterTurn","Eventer ready allocation ->");
		struct Eventer** eventers = malloc(sizeof(struct Eventer*)*character->eventerCount);
		unsigned int allowedeventers = 0;

		DEBUG_PRINT("tbwgCharacterTurn","Eventer control ->");
		for(unsigned int i = 0 ; i < character->eventerCount ; i += 1) {
			if(character->eventers[i]->setReady((void*)(character->eventers[i]), character, tbwgGetWorld())) {
				eventers[allowedeventers] = character->eventers[i];
				allowedeventers += 1;
			}
		}
		DEBUG_PRINT("tbwgCharacterTurn","Choose eventer ->");

		choose = interface->chooseEventer(interface, character->b.ID, turn->allowedEventerTypes, allowedeventers, eventers,
			character->eventerSpendings);

		DEBUG_PRINT("tbwgCharacterTurn","End turn? ->");
		if (choose.specs & TURNPLAY_END_TURN) return;

		DEBUG_PRINT("tbwgCharacterTurn","Choosed eventer fetch ->");
		struct Eventer* eventer = eventers[choose.eventer_th];
		free(eventers);

		if (!eventer->canExecutedNow((void*)eventer, &(data->world), turn->character, choose.requiredInformations, NULL)) {
			continue; // cannot be executed, cancel of eventer using before spending
		}

		if (eventer->baseEnergy > character->e.value || eventer->baseSpellEnergy > character->se.value) {
			continue; // failed use
		}

		if (!useEventerRequirements(&(character->eventerSpendings), eventer->costs)) continue;
		
		// energy spendings
		character->e.value -= eventer->baseEnergy;
		character->se.value -= eventer->baseSpellEnergy;

		eventer->isChoosed += 1;
		eventer->executer((void*)eventer, &(data->world), turn->character, choose.requiredInformations, NULL);
	}

	
	for(unsigned int i = 0 ; i < character->eventerCount ; i += 1) {
		if(character->eventers[i]->isChoosed == 0) 
			character->eventers[i]->notChoosed((void*)(character->eventers[i]), tbwgGetWorld(), character);
	}
}

void tbwgEntityTurn(struct QueueEntityTurn* turn)
{
	struct Entity* entity = turn->entity;
	entity->eventer(entity);
}

void tbwgTurn()
{
	DEBUG_PRINT("tbwgTurn","queue empty check.");
	if (queueIsEmpty(&(data->queue))) {
		struct QueueReorderTurn turn; turn.header.type = QUEUE_REORDER;

		queueAddTurn(&(data->queue), (struct QueueElementHeader*)&turn);
		return;
	}

	DEBUG_PRINT("tbwgTurn","retrieving queue element.");
	struct QueueElementHeader* queueElement = queuePop(&(data->queue));

	DEBUG_PRINT("tbwgTurn","queue check check check check");
	if(queueElement->type == QUEUE_CHARACTER) tbwgCharacterTurn((struct QueueCharacterTurn*)queueElement);
	else if(queueElement->type == QUEUE_REORDER) tbwgReorder();
	else if(queueElement->type == QUEUE_ENTITY) tbwgEntityTurn((struct QueueEntityTurn*)queueElement);
	DEBUG_PRINT("tbwgTurn","check end");

	free(queueElement);
}

int tbwgAddCharacter(struct Character* character)
{
	return worldAddCharacter(&(data->world), character);
}

int tbwgAddEntity(struct Entity* entity)
{
	return worldAddEntity(&(data->world), entity);
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





int tbwgMoveBeing(struct Being* b, iVector pc)
{
	return tbwgPutBeing(b, getiVector(b->position.x + pc.x, b->position.y + pc.y));
}

int tbwgPutBeing(struct Being* b, iVector position)
{

	struct Being* rpl = dimensionGetBeingByPosition(b->dimension, position);
	if(rpl != NULL) {
		if (rpl->collisionFunction(b, rpl) == 0) return 0;
	}


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

	return 1;
}
