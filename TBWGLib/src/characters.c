#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <TBWG/tbwgmanager.h>
#include <stdlib.h> // malloc

struct Stats defaultStats = {0, 0, 0, 0, 0, 1, 0};

//typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

#include <stdio.h>

int characterDefaultHit(void* hitting, struct Character* hitter, struct AttackInfo atk)
{
	printf("HIT is:%i\n",atk.damage);
	if (*(TBWGType*)hitting != TBWG_TYPE_CHARACTER) return 0;

	struct Character* hittingCharacter = hitting;

	chTriggerEffect(hittingCharacter, tbwgGetWorld(), EFFECT_TRIGGER_TYPE_HIT, &atk);

	if((hitter->stats.DEX + atk.additiveStats.DEX > hittingCharacter->stats.DEX) || (atk.specs & ATTACK_NONDODGEABLE)) {
		hittingCharacter->hp.value -= atk.damage;
		return 1;
	}

	return 0;
}

struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position)
{
	struct Character* character = malloc(sizeof(struct Character));

	character->b.tbwgType = TBWG_TYPE_CHARACTER;

	printf("TBWGTYPE:%u == %u\n",*(TBWGType*)character, TBWG_TYPE_CHARACTER);
	
	character->b.code = CHARACTER_DEFAULT;
	character->b.ID = getID();
	character->b.position = position;

	character->b.direction = getfVector(1.0f, 0.0f);

	character->b.dimension = dimension;

	character->baseStats = defaultStats;
	character->stats = defaultStats;

	character->b.visionHardness = 0;

	struct Eye eye = {1.8f, 2.0f, 1.0f};
	character->b.baseEye = eye;
	character->b.eye = eye;

	iValue hp = {10,10};
	iValue e = {5,5};
	iValue se = {0,0};

	character->hp = hp;
	character->e = e;
	character->se = se;

	character->state = 0;

	character->passivePowerCount = 0;
	character->passivePowers = malloc(0);

	struct EventerUses newUses = {0,0,0,0,0};
	character->eventerSpendings = newUses;
	
	character->eventerCount = 2;
	character->eventers = malloc(2*sizeof(struct Eventer));

	character->eventers[0] = getDefaultPunchEventer();
	character->eventers[1] = getDefaultWalkEventer();

	for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		character->b.effects[i] = createList();
	}

	character->b.baseQueue = createQueue();

	struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));
	(*defaultCharTurn) = getBasicCharacterTurn();
	defaultCharTurn->character = character;
	queueAddTurn(&(character->b.baseQueue), (struct QueueElementHeader*)defaultCharTurn);

	character->headHit = character->bodyHit = character->armHit = character->legHit = characterDefaultHit;

	character->controllerInterface = getstdioControllerInterface();

	character->seeCharacter = defaultSeeCharacter;
	character->b.canSeen = defaultCanSeen;
	character->seeWorldEvent = defaultSeeWorldEvent;

	return character;
}

float getVisionHardnessFinal(float visionHardness, float distance);


int defaultSeeCharacter(struct Character* observer, struct Character* target)
{
	return target->b.canSeen((struct Being*)observer, (struct Being*)target);
}

int defaultSeeWorldEvent(struct Character* observer, struct WorldEvent* target)
{
	int visionHardnessCheck = observer->b.eye.level >= getVisionHardnessFinal(target->visionHardness, getiVectorDistance(observer->b.position, target->position));
	int speedCheck = observer->b.eye.speed >= target->disappearingSpeed;
	return visionHardnessCheck && speedCheck;
}

void destroyCharacter(struct Character* chr)
{
}

void chAddEffect(struct Effect* effect, unsigned int effectTriggerType, struct Character* c)
{
	struct EffectListElement elm; elm.effect = effect;
	addElement(&(c->b.effects[effectTriggerType]), &elm, sizeof(elm));
}

void chTriggerEffect(struct Character* chr, struct World* world, unsigned int effectTriggerType, void* relativeInformation)
{
	ITERATE(chr->b.effects[effectTriggerType], effectElm_pure) {
		struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;

		effect->executer((void*)effect, world, chr, relativeInformation);
	}
}