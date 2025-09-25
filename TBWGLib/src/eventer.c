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

struct EventerUses changer(struct EventerUses input)
{
	struct EventerUses result = input;
	int* ch = ((int*)(&result)) + 1;
	for(unsigned int i = 0 ; i < 4 ; i += 1) {
		ch[i] += input.classic;
	}

	result.classic = 0;
	return result;
}

struct EventerUses dechanger(struct EventerUses input)
{
	struct EventerUses result = input;
	int* ch = ((int*)(&result)) + 1;

	for(unsigned int i = 0 ; i < 4 ; i += 1) {
		ch[i] -= 1;
		if(ch[i] < 0) {
			for(unsigned int j = 0 ; j <= i ; j += 1) ch[j] += 1;
			goto outdechanger;
		}
		result.classic += 1;
	}
outdechanger:
	return result;
}

int useEventerRequirements(struct EventerUses* u, struct EventerUses req)
{
	struct EventerUses _u = changer(*u);
	req = changer(req);

	int* ch_u = (int*)(&_u);
	int* ch_r = (int*)(&req);

	for (unsigned int i = 0 ; i < 5 ; i += 1) {
		ch_u[i] -= ch_r[i];
		if(ch_u[i] < 0) return 0;
	}

	*u = _u;

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
	struct EventerUses _a = changer(a), _req = changer(req);
	
	int* __a = (int*)&_a; int* __req = (int*)&_req;
	for (unsigned int i = 0 ; i < 5 ; i += 1) {
		if (__a[i] < __req[i]) return 0;
	}

	return 1;
}


// DEFAULTS

int 
defaultEventerCanExecuted(void* eventer, struct Character* c, struct Tool* tool)
{
	return 1;
}

void 
defaultEventerNotChoosed(void* eventer, struct World* w, struct Character* c)
{
}

int 
defaultEventerCanExecutedNow(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	return 1;
}

void 
defaultEventerExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	return;
}

int 
defaultGetEnergy(struct Eventer* e, struct Character* c, struct World* w)
{
	return 0;
}

int 
defaultSetEventerReady(struct Eventer* e, struct Character* c, struct World* w)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED | STATE_ONGROUND)) == 0;
}

int 
defaultSetEventerReady_chrOnFoot(struct Eventer* e, struct Character* c, struct World* w)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED | STATE_ONGROUND)) == 0;
}

int 
defaultSetEventerReady_chrAwake(struct Eventer* e, struct Character* c, struct World* w)
{
	return (c->state & (STATE_DEAD | STATE_FAINTED)) == 0;
}

int 
defaultSetEventerReady_chrNotDead(struct Eventer* e, struct Character* c, struct World* w)
{
	return (c->state & STATE_DEAD) == 0;
}


struct Eventer*
getDefaultEventer()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = EVENTER_DEFAULT;
	evn->ID = getID();

	evn->baseEnergy = 0;
	evn->baseSpellEnergy = 0;

	evn->combo = 0;

	evn->eventer_type = EVENTER_TYPE_CLASSIC;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.armMove = 0;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = defaultEventerExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;

	for (unsigned int i = 0 ; i < 8 ; i += 1) evn->details[i] = 0;
	return evn;
}