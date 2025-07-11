#include <TBWG/eventer.h>
#include <TBWG/attacks.h>
#include <TBWG/characters.h>
#include <TBWG/tbwgmanager.h>

int max(int a, int b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

int useEventerRequirements(struct EventerUses* u, struct EventerUses req)
{
	int requiredFC = max(0, req.fastcombat - u->fastcombat);
	int requiredM = max(0, req.movement - u->movement);

	int requiredFM = max(0, req.fastmagic - u->fastmagic);

	int requiredClassic = max(0, req.classic + requiredFC + requiredM + requiredFM - u->classic);


	int requiredTM = max(0, req.thoughtmagic - u->thoughtmagic);

	if((requiredClassic == 0 && requiredTM == 0) == 0) return 0;


	u->movement -= req.movement - requiredM;
	u->classic -= req.classic + requiredFC + requiredM + requiredFM;
	u->fastcombat -=  req.fastcombat - requiredFC;
	u->fastmagic -= req.fastmagic - requiredFM;

	u->thoughtmagic -= req.thoughtmagic;

	return 1;
}

void addEventerUses(struct EventerUses* a, struct EventerUses b)
{
	int* target = (int*)a;
	int* value = (int*)&b;

	for(unsigned int i = 0 ; i < sizeof(struct EventerUses)/sizeof(int); i += 1) {
		target[i] += value[i];
	}
}

void updateEventerUses(struct EventerUses* a, struct EventerUses b)
{
	int* target = (int*)a;
	int* value = (int*)&b;

	for(unsigned int i = 0 ; i < sizeof(struct EventerUses)/sizeof(int); i += 1) {
		target[i] = value[i];
	}
}

int checkRequiredEventers(struct EventerUses a, struct EventerUses req)
{
	int requiredFC = max(0, req.fastcombat - a.fastcombat);
	int requiredM = max(0, req.movement - a.movement);

	int requiredFM = max(0, req.fastmagic - a.fastmagic);

	int requiredClassic = max(0, req.classic + requiredFC + requiredM + requiredFM - a.classic);


	int requiredTM = max(0, req.thoughtmagic - a.thoughtmagic);



	return requiredClassic == 0 && requiredTM == 0;
}


// DEFAULTS

int defaultEventerCanExecuted(void* eventer, struct Character*, struct Tool* tool)
{
	return 1;
}

void defaultEventerNotChoosed(void* eventer, struct World*, struct Character*)
{
}

int defaultEventerCanExecutedNow(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	return 1;
}

int defaultGetEnergy(struct Eventer*, struct Character*, struct World*)
{
	return 0;
}

int defaultSetEventerReady(struct Eventer*, struct Character* c, struct World*)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED | STATE_ONGROUND)) == 0;
}

int defaultSetEventerReady_chrOnFoot(struct Eventer*, struct Character* c, struct World*)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED | STATE_ONGROUND)) == 0;
}

int defaultSetEventerReady_chrAwake(struct Eventer*, struct Character* c, struct World*)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED)) == 0;
}

int defaultSetEventerReady_chrNotDead(struct Eventer*, struct Character* c, struct World*)
{
	return (c->state & STATE_DEAD) == 0;
}



#include <stdio.h>

void executerDefaultPunchEventer(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = ATTACK_NONDODGEABLE, .damageType = DAMAGE_BLUDGEONING, .damage = 1};

	struct Character* c_target = dimensionGetCharacterByPosition(user->b.dimension, reqinf.position);
	if (c_target == NULL) return;
	printf("attack is going to be succesfull!\n");

	struct WorldEvent punchMove = getDefaultWorldEvent(user->b.ID, "DEFAULT_PUNCH_MOVE", 1.0f, 0.5f, user->b.position, WORLDEVENT_VISION);
	tbwgStreamWorldEvent(user->b.dimension, punchMove);
	struct WorldEvent punchSound = getDefaultWorldEvent(user->b.ID, "SND_PUNCH", 1.0f, 0.5f, reqinf.position, WORLDEVENT_SOUND);
	tbwgStreamWorldEvent(user->b.dimension, punchSound);
	if(c_target->bodyHit(c_target, (void*)user, atkInfo)) printf("LETSGOOOOO\n");
}

struct Eventer*
getDefaultPunchEventer()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = EVENTER_DEFAULT;
	evn->ID = getID();
	getName(evn->name, "DEFAULT_PUNCH");

	evn->baseEnergy = 1;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,1,0,0};
	costs.fastcombat = 1;
	costs.fastmagic = 0;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = executerDefaultPunchEventer;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}






void defaultWalkEventerExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct WorldEvent walkMove = getDefaultWorldEvent(user->b.ID, "DEFAULT_WALK", 0.0f, 0.5f, user->b.position, WORLDEVENT_VISION);
	tbwgStreamWorldEvent(user->b.dimension, walkMove);

	tbwgPutBeing((struct Being*)user, reqinf.position);
}


struct Eventer* getDefaultWalkEventer()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = EVENTER_DEFAULT;
	evn->ID = getID();
	getName(evn->name, "DEFAULT_WALK");

	evn->baseEnergy = 1;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = defaultWalkEventerExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}