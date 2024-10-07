#include <TBWG/eventer.h>
#include <TBWG/attacks.h>
#include <TBWG/characters.h>

#include <stdio.h>

void executerDefaultPunchEventer(void* eventer, struct World* world, struct Character* user, void* target, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = ATTACK_NONDODGEABLE, .damageType = DAMAGE_BLUDGEONING, .damage = 1};
	if(vt->target_type == TARGET_ONE) {
		struct TargetPosition* tposition = (struct TargetPosition*)target;

		struct Character* c_target = dimensionGetCharacterByPosition(user->dimension, tposition->x, tposition->y);
		if (c_target == NULL) return;
		printf("attack is going to be succesfull!\n");
		if(c_target->bodyHit(c_target, (void*)user, atkInfo)) printf("LETSGOOOOO\n");
	}
}

int executerCanCast(void* eventer, struct Character*, struct Tool* tool)
{
	return 1;
}

void executerNotChoosed(void* eventer, struct World*, struct Character*)
{
}


struct Eventer
getDefaultPunchEventer()
{
	struct Eventer evn;
	evn.eventerCode = EVENTER_DEFAULT;
	evn.ID = getID();

	evn.energy = 1;
	evn.spellEnergy = 0;

	evn.eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn.target_type = TARGET_ONE;

	evn.executer = executerDefaultPunchEventer;
	evn.canCast = executerCanCast;
	evn.notChoosed = executerNotChoosed;
	return evn;
}