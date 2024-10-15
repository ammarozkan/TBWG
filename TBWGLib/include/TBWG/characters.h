#ifndef TBWG_CHARACTERS_H
#define TBWG_CHARACTERS_H

#include <TBWG/essentials.h>
#include <TBWG/attacks.h>
#include <TBWG/controllerInterface.h>
#include <TBWG/stats.h>
#include <TBWG/queue.h>
#include <TBWG/observation.h>
#include <TBWG/maths.h>
#include <TBWG/eventer.h>
#include <TBWG/effects.h>


struct Physiology {
	iValue hp, e, se;
};


#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

#define CHARACTER_DEFAULT 0x00


typedef int (*SeeCharacter)(struct Character* observer, struct Character* target);
typedef int (*SeeWorldEvent)(struct Character* observer, struct WorldEvent* target);
typedef int (*CanSeen)(struct Character* observer, struct Character* target);

struct Character {
	TBWGType tbwgType;
	id_number ID;

	unsigned int characterCode;

	iVector position;
	struct Dimension* dimension;
	fVector direction;

	struct Stats baseStats;
	struct Stats stats;

	struct Eye baseEye;
	struct Eye eye;

	iValue hp, e, se;
	digits32 state;

	struct List seeingResources; // in example gaara's eye, rinnegan bodies.

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	struct EventerUses eventerSpendings;
	size_t eventerCount;
	struct Eventer* eventers;

	struct List effects[EFFECT_TRIGGER_TYPE_COUNT];

	struct Queue baseQueue;

	HitterFunction headHit, bodyHit, armHit, legHit;

	struct ControllerInterface* controllerInterface;

	SeeCharacter seeCharacter;
	SeeWorldEvent seeWorldEvent;
	CanSeen canSeen;
};

struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position);
void destroyCharacter(struct Character*);


int defaultSeeCharacter(struct Character* observer, struct Character* target);
int defaultSeeWorldEvent(struct Character* observer, struct WorldEvent* target);
int defaultCanSeen(struct Character* observer, struct Character* target);

void chAddEffect(struct Effect effect, unsigned int effectTriggerType, struct Character*);
void chTriggerEffect(struct Character* ch, struct World* world, unsigned int effectTriggerType, void* relativeInformation);

#endif /*TBWG_CHARACTERS_H*/
