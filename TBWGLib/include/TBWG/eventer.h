#ifndef TBWG_EVENTER_H
#define TBWG_EVENTER_H

#include <TBWG/essentials.h>
#include <TBWG/tool.h>
#include <TBWG/world.h>
#include <TBWG/maths.h>


struct Eventer;

#define EVENTER_TYPE_CLASSIC (1<<0)
#define EVENTER_TYPE_FASTMAGIC (1<<1)
#define EVENTER_TYPE_FASTCOMBAT (1<<2)


#define EVENTER_REQUIRED_INFORMATION_POSITION (1<<0)
#define EVENTER_REQUIRED_INFORMATION_AREA (1<<1)
#define EVENTER_REQUIRED_INFORMATION_DIRECTION (1<<2)

#define EVENTER_DEFAULT 0x00

struct EventerUses {
	int classic, fastcombat, movement, 
			fastmagic, thoughtmagic;
};

int useEventerRequirements(struct EventerUses*, struct EventerUses requirements);

void addEventerUses(struct EventerUses* a, struct EventerUses b);
int checkRequiredEventers(struct EventerUses, struct EventerUses req);

struct EventerRequiredInformations {
	iVector position;
	iVector position2;
	fVector direction;
	iVector A,B; // area coordinates A and B
};

int executerCanExecuteNow(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool);

#define EVENTER_ENERGYUSE_BASE 0x01
#define EVENTER_ENERGYUSE_BYUSE 0x02

typedef int (*tbwgtypeSetEventerReady)(struct Eventer*, struct Character*, struct World*);

struct Eventer {
	unsigned int eventerCode;
	id_number ID;

	char name[32];

	uint8_t energyValueType;
	int baseEnergy, baseSpellEnergy;
	digits32 eventer_type, required_informations;
	struct EventerUses costs;

	tbwgtypeSetEventerReady setReady;

	int (*canExecutedNow)(void* eventer, struct World*, struct Character*, 
		struct EventerRequiredInformations, struct Tool* tool);

	void (*executer)(void* eventer, struct World*, struct Character*, 
		struct EventerRequiredInformations, struct Tool* tool);

	// when not choosed

	void (*notChoosed)(void* eventer, struct World*, struct Character*);
};

#define TURNPLAY_END_TURN (1<<1)

struct TurnPlay {
	unsigned int eventer_th;
	struct EventerRequiredInformations requiredInformations; // if target is NULL, then choosing failed.
	unsigned int specs;
};

struct Eventer* getDefaultPunchEventer();
struct Eventer* getDefaultWalkEventer();

#endif