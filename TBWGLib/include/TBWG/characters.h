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
#include <TBWG/being.h>


struct Physiology {
	iValue hp, e, se;
};


#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

#define CHARACTER_DEFAULT 0x00


typedef int (*SeeCharacter)(struct Character* observer, struct Character* target);
typedef int (*SeeWorldEvent)(struct Character* observer, struct WorldEvent* target);

typedef int (*EnergyRegener)(struct Character*, int amount);
typedef int (*HealthRegener)(struct Character*, int amount);

struct Character {
	struct Being b;

	struct Stats baseStats;
	struct Stats stats;

	iValue hp, e, se;
	digits32 state;

	struct List seeingResources; // in example gaara's eye, rinnegan bodies.

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	struct EventerUses eventerSpendings;
	size_t eventerCount;
	struct Eventer* *eventers;

	HitterFunction headHit, bodyHit, armHit, legHit;

	EnergyRegener energyRegener;
	HealthRegener healthRegener;

	struct ControllerInterface* controllerInterface;

	SeeCharacter seeCharacter;
	SeeWorldEvent seeWorldEvent;
};

int characterDefaultHit(void* hitting, struct Character* hitter, struct AttackInfo atk);
int characterDefaultEnergyRegener(struct Character* c, int amount);
int characterDefaultHealthRegener(struct Character* c, int amount);

struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position);
void destroyCharacter(struct Character*);


int defaultSeeCharacter(struct Character* observer, struct Character* target);
int defaultSeeWorldEvent(struct Character* observer, struct WorldEvent* target);

void chChangeControllerInterface(struct Character*, struct ControllerInterface* newInterface);

void chAddEffect(struct Effect* effect, unsigned int effectTriggerType, struct Character*);
void chTriggerEffect(struct Character* ch, struct World* world, unsigned int effectTriggerType, void* relativeInformation);
void chUpdateStats(struct Character* ch);

TBWGType* tbwgFindBeingByPosition(struct Dimension* dim, int x, int y);

#endif /*TBWG_CHARACTERS_H*/
