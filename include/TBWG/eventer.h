#include <TBWG/essentials.h>

#define EVENTER_CLASSIC (1<<0)
#define EVENTER_FASTMAGIC (1<<0)
#define EVENTER_FASTCOMBAT (1<<0)


#define TARGET_ONE (1<<0)
#define TARGET_AREA (1<<1)
#define TARGET_POSITION (1<<2)

struct Eventer {
	unsigned int ID;
	int energy, spellEnergy;
	digits32 eventer_type, target_type;

	void (*executer)(void* eventer, struct World*, struct Character*, void* target);
	void (*notChoosed)(void* eventer, struct World*, struct Character*);
};