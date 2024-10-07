#include <TBWG/essentials.h>
#include <TBWG/tool.h>
#include <TBWG/world.h>

#define EVENTER_TYPE_CLASSIC (1<<0)
#define EVENTER_TYPE_FASTMAGIC (1<<1)
#define EVENTER_TYPE_FASTCOMBAT (1<<2)


#define TARGET_ONE (1<<0)
#define TARGET_AREA (1<<1)
#define TARGET_POSITION (1<<2)

#define EVENTER_DEFAULT 0x00

struct Eventer {
	unsigned int eventerCode;
	id_number ID;

	int energy, spellEnergy;
	digits32 eventer_type, target_type;

	void (*executer)(void* eventer, struct World*, struct Character*, 
		void* target, struct Tool* tool);
	int (*canCast)(void* eventer, struct Character*, struct Tool* tool);
	void (*notChoosed)(void* eventer, struct World*, struct Character*);
};

struct TargetPosition {
	int x,y;
};

struct TargetArea {
	int x1, x2, y1, y2;
};

struct TargetDirectedArea {
	int x1, x2, y1, y2;
	int dirx, diry;
};

struct EventerChoose {
	unsigned int eventer_th;
	void* target; // if target is NULL, then choosing failed.
};

struct Eventer getDefaultPunchEventer();