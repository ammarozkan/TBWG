#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/randomness.h> // tbwgNumber
#include <stdlib.h> // malloc

struct Stats defaultStats = {0, 0, 0, 0, 0, 1, 0};

//typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

int characterDefaultHit(void* hitting, struct Character* hitter, struct AttackInfo atk)
{
	if (*(TBWGType*)hitting != TBWG_TYPE_CHARACTER) return 0;

	struct Character* hittingCharacter = hitting;

	chTriggerEffect(hittingCharacter, tbwgGetWorld(), EFFECT_TRIGGER_TYPE_HIT, &atk);

	int hitterDEX = hitter->stats.DEX + atk.additiveStats.DEX;

	int seecontrol = canSeeCharacter(hitting, hitter) || tbwgGetRandomed_2(60, 10);

	if((tbwgRandomPercentageIncrease(hitterDEX, 0, 60) > hittingCharacter->stats.DEX) || (atk.specs & ATTACK_NONDODGEABLE) || !seecontrol) {
		hittingCharacter->hp.value -= atk.damage;
		return 1;
	}

	return 0;
}

int characterDefaultEnergyRegener(struct Character* c, int amount)
{
	return addToiValue(&(c->e), amount);
}

int characterDefaultHealthRegener(struct Character* c, int amount)
{
	return addToiValue(&(c->hp), amount);
}

struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position)
{
	struct Character* character = malloc(sizeof(struct Character));

	character->b.tbwgType = TBWG_TYPE_CHARACTER;
	
	character->b.code = CHARACTER_DEFAULT;
	character->b.ID = getID();
	character->b.position = position;

	character->b.direction = getfVector(1.0f, 0.0f);

	character->b.dimension = dimension;

	character->b.collisionFunction = beingDefaultOneWayCollision;

	character->baseStats = defaultStats;
	character->stats = defaultStats;

	character->b.visionHardness = 0;

	struct Eye eye = {1.8f, 1.0f, 0.1f, 1.0f, 0.3f};
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
	character->energyRegener = characterDefaultEnergyRegener;
	character->healthRegener = characterDefaultHealthRegener;

	character->controllerInterface = getDefaultControllerInterface();

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
	if (target->eventStreamingType == WORLDEVENT_VISION) {
		int visionHardnessCheck = observer->b.eye.level >= getVisionHardnessFinal(target->detailLevel, getiVectorDistance(observer->b.position, target->position));
		int speedCheck = observer->b.eye.speed <= target->disappearingSpeed;
		return visionHardnessCheck && speedCheck;
	} else if(target->eventStreamingType == WORLDEVENT_SOUND) {
		int hearingCheck = observer->b.eye.hearingLevel >= target->detailLevel;
		int speedCheck = observer->b.eye.hearingSpeed <= target->disappearingSpeed;
		return hearingCheck && speedCheck;
	}
	
	return 0;
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
	void* theGreaterBull = NULL;
	ITERATE(chr->b.effects[effectTriggerType], effectElm_pure) {
		if(theGreaterBull!=NULL) {
			free(theGreaterBull);
			theGreaterBull = NULL;
		}

		struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;
		
		effect->executer((void*)effect, world, chr, relativeInformation);
		
		if(effect->time != -1) {
			effect->time -= 1;
			if (effect->time <= 0) effect->willberemoved = 1;
		}

		if (effect->willberemoved == 1) {
			theGreaterBull = popElement(&(chr->b.effects[effectTriggerType]), effectElm_pure);
		}
	}
}

void chUpdateStats(struct Character* ch)
{
	ch->stats = ch->baseStats;
	for (unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) ITERATE(ch->b.effects[i], effectElm_pure) {
		struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;

		ch->stats = tbwgSumStats(ch->stats, effect->givenStats);
	}
}


void chChangeControllerInterface(struct Character* chr, struct ControllerInterface* newInterface)
{
	free(chr->controllerInterface);
	chr->controllerInterface = newInterface;
}