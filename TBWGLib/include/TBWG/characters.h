#include <TBWG/essentials.h>

struct Stats {
	int STR, DEX, CNS, WIS, SCS:
};


#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

struct Character;
struct AttackInfo;

#define CHARACTER_DEFAULT 0x00

struct Character {
	unsigned int characterCode = CHARACTER_DEFAULT;
	id_number ID;

	int x,y;

	struct Stats baseStats;
	struct Stats stats;

	int maxhp, maxe, maxse;
	int hp, e, se;
	unsigned int state;

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	size_t eventerCount;
	struct Eventer* eventers;

	struct List effects;

	hitterFunction headHit, bodyHit, armHit, legHit;

	ControllerInterface* controllerInterface;
};