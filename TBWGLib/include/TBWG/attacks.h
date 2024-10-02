struct Attackinfo;

typedef int (*hitterFunction)(struct Character* self, struct Character* hitter,
	struct AttackInfo);

#define ATTACK_NONDODGEABLE (1<<0)

#define DAMAGE_BLUDGEONING 0x01

struct AttackInfo {
	struct Stats additiveStats;
	unsigned int specs;

	enum DamageType damageType;
	int damage;
};