#include <TBWG/effects.h>
#include <TBWG/essentials.h>
#include <TBWG/stats.h>
#include <TBWG/attacks.h>

void defaultEffectExecuter(void* effectptr, struct World* w, struct Character* entity, void* relativeInformation)
{
	return;
}



#include <stdio.h>



// this testing effect erases bludgeoning damage 2 times, then get passes one. then again earns 2 chances to erase.

struct TestingHitEffect {
	struct Effect effect;
	int dodgeCount;
};

void testingHitEffectExecuter(void* effectptr, struct World* w, struct Character* entity, void* relativeInformation)
{
	struct TestingHitEffect* f = (struct TestingHitEffect*)effectptr;

	struct AttackInfo* atk = (struct AttackInfo*) relativeInformation;

	if(atk->damageType == DAMAGE_BLUDGEONING && f->dodgeCount != 2) {
		printf("ERASING BLUDGEONING DAMAGE!\n");
		atk->damage = 0;
	}

	f->dodgeCount += 1;
	if(f->dodgeCount == 2) f->dodgeCount = 0;
}

void defaultEffectSetReady(void* effectptr, struct Character* entity)
{

}

struct Effect* getTestingHitEffect(void* effector)
{
	printf("Testing effect creation.\n");
	struct TestingHitEffect* f = GET_SIZED_STRUCT(TestingHitEffect);
	f->effect.ID = getID();
	f->effect.code = 0;

	printf("Effector type retrieving.\n");
	TBWGType effectorType = TBWG_TYPE_UNKNOWN;
	if(effector != NULL) effectorType = *((TBWGType*)effector);

	f->effect.effectorType = effectorType;
	f->effect.effector = effector;

	f->effect.time = 3;
	f->effect.willberemoved = 0;

	f->effect.effectSpecs = 0;

	struct Stats s = {0, 0, 0, 0, 0, 0, 0};
	f->effect.givenStats = s;

	f->effect.executer = testingHitEffectExecuter;
	f->effect.onremove = defaultEffectExecuter;
	f->effect.setReady = defaultEffectSetReady;
	f->dodgeCount = 0;

	for (unsigned int i = 0 ; i < 8 ; i += 1) {
		f->effect.details[i] = 0;
	}
	return (struct Effect*)f;
}