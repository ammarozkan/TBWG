#include <TBWG/essentials.h>
#include <TBWG/maths.h>
#include <TBWG/randomness.h>
#include "can.h"

void tgfLookExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct WorldEvent lookMove = getDefaultWorldEvent(user->b.ID, "MVM_LOOK", 0.0f, 0.5f, user->b.position, WORLDEVENT_VISION);
	tbwgStreamWorldEvent(user->b.dimension, lookMove);

	user->b.direction = reqinf.direction;
}

struct Eventer* getTgfLook()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_LOOK;
	evn->ID = getID();
	getName(evn->name, TGF_LOOK_NAME);

	evn->baseEnergy = 1;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_CLASSIC;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_DIRECTION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfLookExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}


//
//
//
// WALK
//
//
//

int tgfCanWalk(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* evn = (struct Eventer*) eventer;

	iVector pos = user->b.position;
	iVector target = reqinf.position;

	float maxWalk = ((float)(user->stats.SPD) * 0.1f);

	return getiVectorDistance(pos,target)*0.5f <= maxWalk;
}

void tgfWalkExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* evn = (struct Eventer*) eventer;
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.0f, 0.5f, user->b.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.0f, 0.5f, reqinf.position, WORLDEVENT_SOUND));
	tbwgPutBeing((struct Being*)user, reqinf.position);
}

struct Eventer* getTgfWalk()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_WALK;
	evn->ID = getID();
	getName(evn->name, TGF_WALK_NAME);

	evn->baseEnergy = 2;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_CLASSIC;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;
	evn->executer = tgfWalkExecuter;
	evn->canExecutedNow = tgfCanWalk;
	return evn;
}

//
//
//
// PUNCH
//
//
//

void tgfPunchExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};

	if(getiVectorDistance(user->b.position,reqinf.position));

	int r_dmg = tbwgGetRandomed_2(50,10);

	int damage = 5*user->stats.STR;
	if (r_dmg == 0) damage += 0;
	else if(r_dmg == 1) damage += 1*user->stats.STR;

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = 0, .damageType = DAMAGE_BLUDGEONING, .damage = damage};

	struct Character* c_target = dimensionGetCharacterByPosition(user->b.dimension, reqinf.position.x, reqinf.position.y);
	if (c_target == NULL) return;

	struct WorldEvent punchMove = getDefaultWorldEvent(user->b.ID, "MVM_PUNCH", 1.0f, 0.5f, user->b.position, WORLDEVENT_VISION);
	tbwgStreamWorldEvent(user->b.dimension, punchMove);
	struct WorldEvent punchSound = getDefaultWorldEvent(user->b.ID, "SND_PUNCH", 1.0f, 0.5f, reqinf.position, WORLDEVENT_SOUND);
	tbwgStreamWorldEvent(user->b.dimension, punchSound);
	if(c_target->bodyHit(c_target, (void*)user, atkInfo)) ;
}

struct Eventer* getTgfPunch()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_PUNCH;
	evn->ID = getID();
	getName(evn->name, TGF_PUNCH_NAME);

	evn->baseEnergy = 6;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.fastcombat = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfPunchExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}

//
//
//
// REST
//
//
//

void tgfRestExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	int rest_amount = user->stats.CNS/2;
	int ascending = user->energyRegener(user, rest_amount);
	
	
	user->healthRegener(user, ascending);

}

struct Eventer* getTgfRest()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_REST;
	evn->ID = getID();
	getName(evn->name, TGF_REST_NAME);

	evn->baseEnergy = 0;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_RESTING;
	evn->required_informations = 0;
	struct EventerUses costs = {1,0,0,0,0};
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfRestExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}


//
//
//
// HUMAN
//
//
//

iValue getHPByCONS(int constution)
{
	iValue hp = {.value = constution*10, .max = constution*10};
	return hp;
}

struct Stats tgfDefaultStats = {10, 10, 10, 10, 0, 6, 2};

struct Character* createDefaultHuman(struct Dimension* dimension, iVector position)
{
	struct Character* character = malloc(sizeof(struct Character));

	character->b.tbwgType = TBWG_TYPE_CHARACTER;
	
	character->b.code = TGF_CHR_HUMAN;
	character->b.ID = getID();
	character->b.position = position;

	character->b.direction = getfVector(1.0f, 0.0f);

	character->b.dimension = dimension;

	character->baseStats = tgfDefaultStats;
	character->stats = tgfDefaultStats;

	character->b.visionHardness = 0;

	struct Eye eye = {1.8f, 2.0f, 1.0f};
	character->b.baseEye = eye;
	character->b.eye = eye;

	iValue hp = getHPByCONS(character->stats.CNS);
	iValue e = {40,40};
	iValue se = {0,0};

	character->hp = hp;
	character->e = e;
	character->se = se;

	character->state = 0;

	character->passivePowerCount = 0;
	character->passivePowers = malloc(0);

	struct EventerUses newUses = {0,0,0,0,0};
	character->eventerSpendings = newUses;
	
	character->eventerCount = 4;
	character->eventers = malloc(4*sizeof(struct Eventer));

	character->eventers[0] = getTgfPunch();
	character->eventers[1] = getTgfWalk();
	character->eventers[2] = getTgfLook();
	character->eventers[3] = getTgfRest();

	for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		character->b.effects[i] = createList();
	}

	character->b.baseQueue = createQueue();

	struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));
	(*defaultCharTurn) = getBasicCharacterTurn();
	defaultCharTurn->character = character;
	queueAddTurn(&(character->b.baseQueue), (struct QueueElementHeader*)defaultCharTurn);

	character->headHit = character->bodyHit = character->armHit = character->legHit = characterDefaultHit;
	character->energyRegener = characterDefaultEnergyRegener;
	character->healthRegener = characterDefaultHealthRegener;

	character->controllerInterface = getDefaultControllerInterface();

	character->seeCharacter = defaultSeeCharacter;
	character->b.canSeen = defaultCanSeen;
	character->seeWorldEvent = defaultSeeWorldEvent;

	return character;
}