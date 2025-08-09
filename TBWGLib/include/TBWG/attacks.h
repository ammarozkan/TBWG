#ifndef TBWG_ATTACKS_H
#define TBWG_ATTACKS_H

#include <TBWG/stats.h>
#include <TBWG/maths.h>

#define ATTACK_NONDODGEABLE (1<<0)
#define ATTACK_AREA_ATTACK (1<<1)
#define ATTACK_HITTED (1<<12)

#define DAMAGE_BLUDGEONING 0x01
#define DAMAGE_WRESTLE 0x02
#define DAMAGE_CUTTING 0x03
#define DAMAGE_BLEEDING 0x04
#define DAMAGE_TOUCH_SPECIFIC 0x05 // trying to touch to a specific location, it should get harder

struct AttackInfo {
	struct Stats additiveStats;
	unsigned int specs;

	unsigned int damageType;
	int damage;
	iArea attackArea;
};

struct Character;

typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

#endif /*TBWG_ATTACKS_H*/