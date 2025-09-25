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
#include <TBWG/system/comboFunctions.h>


struct Physiology {
	iValue hp, e, se;
};


#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)
#define STATE_ONGROUND (1<<3)

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

	struct List seeingResources; // other outsider eyes.

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	struct EventerUses eventerSpendings;
	size_t eventerCount;
	struct Eventer* *eventers;

	struct ComboFunction headHit, bodyHit, armHit, legHit;
	//struct ComboFunction headAttack, bodyAttack, armAttack, legAttack;

	struct ControllerInterface* controllerInterface;

	struct ComboFunction energyRegener;
	struct ComboFunction spellEnergyRegener;
	struct ComboFunction healthRegener;
	struct ComboFunction seeCharacter;
	struct ComboFunction seeWorldEvent;
};


struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position);
void chDestroy(struct Character*);
void chChangeControllerInterface(struct Character*, struct ControllerInterface* newInterface);
void chAddEffect(struct Effect* effect, unsigned int effectTriggerType, struct Character*);
void chTriggerEffect(struct Character* ch, struct World* world, unsigned int effectTriggerType, void* relativeInformation);
void chRefreshEffect(struct Character* chr, struct World* world, unsigned int effectTriggerType, void* relativeInformation);
void chUpdateStats(struct Character* ch);

TBWGType* tbwgFindBeingByPosition(struct Dimension* dim, int x, int y);


// default solo functions

int characterDefaultEnergyRegener(void*ptr, struct ComboPositionlessInstructors*, struct Character* c, int amount);
int characterDefaultSpellEnergyRegener(void*ptr, struct ComboPositionlessInstructors*, struct Character* c, int amount);
int characterDefaultHealthRegener(void*ptr, struct ComboPositionlessInstructors*, struct Character* c, int amount);
struct Character* characterDefaultSeeCharacter(void*ptr, struct ComboPositionlessInstructors*, struct Character* observer, struct Character* target);
struct WorldEvent* characterDefaultSeeWorldEvent(void*ptr, struct ComboPositionlessInstructors*, struct Character* observer, struct WorldEvent* target);
struct AttackInfo characterDefaultHit(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* harmed, void* hitter, struct AttackInfo atk);



// outside executers

int chEnergyRegener(struct Character*, int amount);
int chSpellEnergyRegener(struct Character*, int amount);
int chHealthRegener(struct Character*, int amount);
int chSeeCharacter(struct Character* observer, struct Character* target);
int chSeeWorldEvent(struct Character* observer, struct WorldEvent* worldEvent);
struct AttackInfo chHit(struct Character* harmed, void* hitter, struct AttackInfo attack);
struct AttackInfo chHitBody(struct Character* harmed, void* hitter, struct AttackInfo attack);
struct AttackInfo chHitHead(struct Character* harmed, void* hitter, struct AttackInfo attack);
struct AttackInfo chHitArm(struct Character* harmed, void* hitter, struct AttackInfo attack);
struct AttackInfo chHitLeg(struct Character* harmed, void* hitter, struct AttackInfo attack);

#endif /*TBWG_CHARACTERS_H*/
