#include <TBWG/being.h>


int defaultCanSeen(struct Being* observer, struct Being* target)
{
	return observer->eye.level >= target->visionHardness;
}

struct Effect* beingGetEffectByCode(struct Being* b, uint32_t effectCode)
{
	for(unsigned int effectType = 0 ; effectType < EFFECT_TRIGGER_TYPE_COUNT ; effectType += 1)
	ITERATE(b->effects[effectType], effectElm_pure) {

		struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;
		if (effect->code == effectCode) return effect;
	}
	return NULL;
}


int beingDefaultOneWayCollision(struct Being* t, struct Being* walker)
{
	return 0; // cant walk over
}


int beingDefaultTwoWayCollision(struct Being* t, struct Being* walker)
{
	return !walker->collisionFunction(walker, t); // can walk over if other cant walkened over
}