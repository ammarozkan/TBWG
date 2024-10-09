#include <TBWG/eventer.h>
#include <TBWG/attacks.h>
#include <TBWG/characters.h>
#include <TBWG/tbwgmanager.h>

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
	evn.name = "DEFAULT_PUNCH";

	evn.energy = 1;
	evn.spellEnergy = 0;

	evn.eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn.required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;

	evn.executer = executerDefaultPunchEventer;
	evn.canCast = executerCanCast;
	evn.notChoosed = executerNotChoosed;
	return evn;
}