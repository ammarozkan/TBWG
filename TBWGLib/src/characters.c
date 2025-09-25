#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/randomness.h> // tbwgNumber
#include <stdlib.h> // malloc

struct Stats defaultStats = {0, 0, 0, 0, 0, 1, 0};

//typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

int characterDefaultEnergyRegener(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* c, int amount)
{
	return addToiValue(&(c->e), amount);
}

int characterDefaultSpellEnergyRegener(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* c, int amount)
{
	return addToiValue(&(c->se), amount);
}

int characterDefaultHealthRegener(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* c, int amount)
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

	character->b.tool = NULL;

	character->baseStats = defaultStats;
	character->stats = defaultStats;

	character->b.visionHardness = 0;

	struct Eye eye = {1.8f, 1.0f, 0.1f, 1.0f, 0.3f};
	character->b.baseEye = eye;
	character->b.eye = eye;
	character->seeingResources = createList();

	character->hp = getiValue(10,10);
	character->e = getiValue(5,5);
	character->se = getiValue(0,0);

	character->state = 0;

	character->passivePowerCount = 0;
	character->passivePowers = malloc(0);

	struct EventerUses newUses = {0,0,0,0,0};
	character->eventerSpendings = newUses;
	
	character->eventerCount = 0;
	character->eventers = malloc(0*sizeof(struct Eventer));

	for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		character->b.effects[i] = createList();
	}

	character->b.baseQueue = createQueue();

	struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));
	(*defaultCharTurn) = getBasicCharacterTurn();
	defaultCharTurn->character = character;
	queueAddTurn(&(character->b.baseQueue), (struct QueueElementHeader*)defaultCharTurn);

	character->headHit = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_ppA, (ComboSolo)characterDefaultHit, NULL);
	character->bodyHit = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_ppA, (ComboSolo)characterDefaultHit, NULL);
	character->armHit = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_ppA, (ComboSolo)characterDefaultHit, NULL);
	character->legHit = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_ppA, (ComboSolo)characterDefaultHit, NULL);

	character->controllerInterface = getDefaultControllerInterface();

	character->energyRegener = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_pi, (ComboSolo)characterDefaultEnergyRegener, NULL);
	character->spellEnergyRegener = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_pi, (ComboSolo)characterDefaultSpellEnergyRegener, NULL);
	character->healthRegener = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_pi, (ComboSolo)characterDefaultHealthRegener, NULL);
	character->seeCharacter = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_pp, (ComboSolo)characterDefaultSeeCharacter, NULL);
	character->seeWorldEvent = tbwgGetComboFunction2((ComboExec)tbwgComboFunctionExecuter_pp, (ComboSolo)characterDefaultSeeWorldEvent, NULL);

	character->b.canSeen = defaultCanSeen;

	return character;
}

float getVisionHardnessFinal(float visionHardness, float distance);


struct Character* characterDefaultSeeCharacter(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* observer, struct Character* /*STREAM*/ target)
{
	if(target != NULL && target->b.canSeen((struct Being*)observer, (struct Being*)target)) return target;
	return NULL;
}

struct WorldEvent* characterDefaultSeeWorldEvent(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* observer, struct WorldEvent* target)
{
	int success = 0;
	if (target->eventStreamingType == WORLDEVENT_VISION) {
		int visionHardnessCheck = observer->b.eye.level >= getVisionHardnessFinal(target->detailLevel, getiVectorDistance(observer->b.position, target->position));
		int speedCheck = observer->b.eye.speed <= target->disappearingSpeed;
		success = visionHardnessCheck && speedCheck;
	} else if(target->eventStreamingType == WORLDEVENT_SOUND) {
		int hearingCheck = observer->b.eye.hearingLevel >= target->detailLevel;
		int speedCheck = observer->b.eye.hearingSpeed <= target->disappearingSpeed;
		success = hearingCheck && speedCheck;
	}

	if (success) return target;
	else return NULL;
}


#include <stdio.h>

struct AttackInfo characterDefaultHit(void*ptr, struct ComboPositionlessInstructors* instr, struct Character* harmed, void* hitter, struct AttackInfo atk)
{
	struct Character* hitterCharacter = hitter;

	chTriggerEffect(harmed, tbwgGetWorld(), EFFECT_TRIGGER_TYPE_HIT, &atk);
	
	if (hitterCharacter == NULL) {
		if((atk.additiveStats.DEX > harmed->stats.DEX) || (atk.specs & ATTACK_NONDODGEABLE) || (atk.damageType == DAMAGE_BLEEDING)) {
			goto gohit;
		}
	}

	int hitterDEX = hitterCharacter->stats.DEX + atk.additiveStats.DEX;

	if (atk.damageType == DAMAGE_TOUCH_SPECIFIC) hitterDEX -= 1;

	int seecontrol = canSeeCharacter(harmed, hitterCharacter) || tbwgGetRandomed_2(60, 10);

	if(tbwgRandomPercentageIncrease(hitterDEX, -30, 30) > harmed->stats.DEX) {
		printf("JOEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
		printf("dexterity hit.\n");
		goto gohit;
	}

	if((atk.specs & ATTACK_NONDODGEABLE) || seecontrol != 0 || atk.damageType == DAMAGE_BLEEDING) {
	//if((hitterDEX > harmed->stats.DEX) || (atk.specs & ATTACK_NONDODGEABLE) || !seecontrol || atk.damageType == DAMAGE_BLEEDING) {
		goto gohit;
	}
	goto gohitcancel;

gohit:
	harmed->hp.value -= atk.damage;
	atk.damage = 0;
	atk.specs = atk.specs | ATTACK_HITTED;
	return atk;
gohitcancel:
	return atk;
}

void chDestroy(struct Character* chr)
{
}

void chAddEffect(struct Effect* effect, unsigned int effectTriggerType, struct Character* c)
{
	struct EffectListElement elm; elm.effect = effect;
	addElement(&(c->b.effects[effectTriggerType]), &elm, sizeof(elm));
	effect->setReady((void*)effect, c);
}

void chRefreshEffect(struct Character* chr, struct World* world, unsigned int effectTriggerType, void* relativeInformation)
{
	ITERATE_FAKE(chr->b.effects[effectTriggerType], effectElm_pure) {
		struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;

		if (effect->willberemoved == 1) {
			effect->onremove((void*)effect, world, chr, relativeInformation);
			ITERATION_DESTROY(chr->b.effects[effectTriggerType], effectElm_pure);
			DEBUG_PRINT("chRefreshEffect","ITERATION_DESTROY call.");
			continue;
		}

		effectElm_pure = effectElm_pure->next;
	}
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
			effect->onremove((void*)effect, world, chr, relativeInformation);
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


// classic executors

int chEnergyRegener(struct Character* c, int amount)
{
	struct ComboFunction cf_i = c->energyRegener;
	return tbwgComboFunctionExecuter_pi(&cf_i, c, amount);
}

int chSpellEnergyRegener(struct Character* c, int amount)
{
	struct ComboFunction cf_i = c->spellEnergyRegener;
	return tbwgComboFunctionExecuter_pi(&cf_i, c, amount);
}

int chHealthRegener(struct Character* c, int amount)
{
	struct ComboFunction cf_i = c->healthRegener;
	return tbwgComboFunctionExecuter_pi(&cf_i, c, amount);
}

int chSeeCharacter(struct Character* c, struct Character* target)
{
	struct ComboFunction cf_i = c->seeCharacter;
	return tbwgComboFunctionExecuter_pp(&cf_i, (void*)c, (void*)target) != NULL;
}

int chSeeWorldEvent(struct Character* c, struct WorldEvent* worldEvent)
{
	struct ComboFunction cf_i = c->seeWorldEvent;
	return tbwgComboFunctionExecuter_pp(&cf_i, (void*)c, (void*)worldEvent) != NULL;
}

struct AttackInfo chHit(struct Character* harmed, void* hitter, struct AttackInfo attack)
{
	struct ComboFunction cf_i = harmed->bodyHit;
	return tbwgComboFunctionExecuter_ppA(&cf_i, harmed, hitter, attack);
}

struct AttackInfo chHitBody(struct Character* harmed, void* hitter, struct AttackInfo attack)
{
	struct ComboFunction cf_i = harmed->bodyHit;
	return tbwgComboFunctionExecuter_ppA(&cf_i, harmed, hitter, attack);
}

struct AttackInfo chHitHead(struct Character* harmed, void* hitter, struct AttackInfo attack)
{
	struct ComboFunction cf_i = harmed->headHit;
	return tbwgComboFunctionExecuter_ppA(&cf_i, harmed, hitter, attack);
}

struct AttackInfo chHitArm(struct Character* harmed, void* hitter, struct AttackInfo attack)
{
	struct ComboFunction cf_i = harmed->armHit;
	return tbwgComboFunctionExecuter_ppA(&cf_i, harmed, hitter, attack);
}

struct AttackInfo chHitLeg(struct Character* harmed, void* hitter, struct AttackInfo attack)
{
	struct ComboFunction cf_i = harmed->legHit;
	return tbwgComboFunctionExecuter_ppA(&cf_i, harmed, hitter, attack);
}