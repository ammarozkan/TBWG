#ifndef TBWG_EVENTER_H
#define TBWG_EVENTER_H

#include <TBWG/essentials.h>
#include <TBWG/tool.h>
#include <TBWG/world.h>
#include <TBWG/maths.h>

#define EVENTER_TYPE_CLASSIC (1<<0)
#define EVENTER_TYPE_FASTMAGIC (1<<1)
#define EVENTER_TYPE_FASTCOMBAT (1<<2)


#define TARGET_ONE (1<<0)
#define TARGET_AREA (1<<1)
#define TARGET_POSITION (1<<2)

#define EVENTER_REQUIRED_INFORMATION_POSITION (1<<0)
#define EVENTER_REQUIRED_INFORMATION_AREA (1<<1)
#define EVENTER_REQUIRED_INFORMATION_DIRECTION (1<<2)

#define EVENTER_DEFAULT 0x00

struct EventerUses {
	unsigned int classic, fastmagic, fastcombat, thoughtmagic;
};

struct EventerRequiredInformations {
	iVector position;
	fVector direction;
	iVector A,B; // area coordinates A and B
};

struct Eventer {
	unsigned int eventerCode;
	id_number ID;

	char* name;

	int energy, spellEnergy;
	digits32 eventer_type, required_informations;
	struct EventerUses costs;
	int usedInThisTurn; // will be set to zero when turn is beginning newly.

	void (*executer)(void* eventer, struct World*, struct Character*, 
		struct EventerRequiredInformations, struct Tool* tool);
	int (*canCast)(void* eventer, struct Character*, struct Tool* tool);
	void (*notChoosed)(void* eventer, struct World*, struct Character*);
};

#define TURNPLAY_END_TURN (1<<1)

struct TurnPlay {
	unsigned int eventer_th;
	struct EventerRequiredInformations requiredInformations; // if target is NULL, then choosing failed.
	unsigned int specs;
};

struct Eventer getDefaultPunchEventer();

#endif