#ifndef TBWG_CHARACTERS_H
#define TBWG_CHARACTERS_H

#include <TBWG/essentials.h>
#include <TBWG/attacks.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/stats.h>
#include <TBWG/queue.h>
#include <TBWG/observation.h>


struct Physiology {
	iValue hp, e, se;
};


#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

#define CHARACTER_DEFAULT 0x00

struct Character {
	TBWGType tbwgType;

	unsigned int characterCode;
	id_number ID;

	int x,y; struct Dimension* dimension;

	struct Stats baseStats;
	struct Stats stats;

	iValue hp, e, se;
	digits32 state;

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	size_t eventerCount;
	struct Eventer* eventers;

	struct List effects;

	struct Queue baseQueue;

	HitterFunction headHit, bodyHit, armHit, legHit;

	struct ControllerInterface* controllerInterface;
};

struct Character* createDefaultCharacter(struct Dimension* dimension);
void destroyCharacter(struct Character*);

#endif /*TBWG_CHARACTERS_H*/