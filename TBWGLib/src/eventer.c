#include <TBWG/eventer.h>
#include <TBWG/attacks.h>
#include <TBWG/characters.h>
#include <TBWG/tbwgmanager.h>

void addEventerUses(struct EventerUses* a, struct EventerUses b)
{
	unsigned int* target = (unsigned int*)a;
	unsigned int* value = (unsigned int*)&b;

	for(unsigned int i = 0 ; i < sizeof(struct EventerUses)/sizeof(unsigned int); i += 1) {
		target[i] += value[i];
	}
}

int checkRequiredEventers(struct EventerUses a, struct EventerUses req)
{
	unsigned int* a_ = (unsigned int*)&a;
	unsigned int* req_ = (unsigned int*)&req;

	for(unsigned int i = 0 ; i < sizeof(struct EventerUses)/sizeof(unsigned int); i += 1)
		if (a_[i] < req_[i]) return 0;

	return 1;
}


int executerCanCast(void* eventer, struct Character*, struct Tool* tool)
{
	return 1;
}

void executerNotChoosed(void* eventer, struct World*, struct Character*)
{
}




#include <stdio.h>

void executerDefaultPunchEventer(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = ATTACK_NONDODGEABLE, .damageType = DAMAGE_BLUDGEONING, .damage = 1};

	struct Character* c_target = dimensionGetCharacterByPosition(user->dimension, reqinf.position.x, reqinf.position.y);
	if (c_target == NULL) return;
	printf("attack is going to be succesfull!\n");

	struct WorldEvent punchMove = {"DEFAULT_PUNCH_MOVE", 1.0f, 0.5f, user->position};
	tbwgStreamWorldEvent(user->dimension, punchMove);
	if(c_target->bodyHit(c_target, (void*)user, atkInfo)) printf("LETSGOOOOO\n");
}


struct Eventer
getDefaultPunchEventer()
{
	struct Eventer evn;
	evn.eventerCode = EVENTER_DEFAULT;
	evn.ID = getID();
	evn.name = "DEFAULT_PUNCH";

	evn.energy = 1;
	evn.spellEnergy = 0;

	evn.eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn.required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,1,0,0};
	costs.fastcombat = 1;
	costs.fastmagic = 0;
	evn.costs = costs;

	evn.executer = executerDefaultPunchEventer;
	evn.canCast = executerCanCast;
	evn.notChoosed = executerNotChoosed;
	return evn;
}