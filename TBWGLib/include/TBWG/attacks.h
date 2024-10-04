#ifndef TBWG_ATTACKS_H
#define TBWG_ATTACKS_H

#include <TBWG/stats.h>

#define ATTACK_NONDODGEABLE (1<<0)

#define DAMAGE_BLUDGEONING 0x01

struct AttackInfo {
	struct Stats additiveStats;
	unsigned int specs;

	unsigned int damageType;
	int damage;
};

struct Character;

typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

#endif /*TBWG_ATTACKS_H*/