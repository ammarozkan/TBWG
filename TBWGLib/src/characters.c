#include <TBWG/characters.h>
#include <TBWG/eventer.h>
#include <stdlib.h> // malloc

struct Stats defaultStats = {0, 0, 0, 0, 0, 1, 2.8f, 1.0f, 0.0f, 1.0f};

//typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

#include <stdio.h>

int characterDefaultHit(void* hitting, struct Character* hitter, struct AttackInfo atk)
{
	printf("HIT is:%i\n",atk.damage);
	if (*(TBWGType*)hitting != TBWG_TYPE_CHARACTER) return 0;

	struct Character* hittingCharacter = hitting;
	if((hitter->stats.DEX + atk.additiveStats.DEX > hittingCharacter->stats.DEX) || (atk.specs & ATTACK_NONDODGEABLE)) {
		hittingCharacter->hp.value -= atk.damage;
		return 1;
	}

	return 0;
}

struct Character* createDefaultCharacter(struct Dimension* dimension, iVector position)
{
	struct Character* character = malloc(sizeof(struct Character));

	character->tbwgType = TBWG_TYPE_CHARACTER;

	printf("TBWGTYPE:%u == %u\n",*(TBWGType*)character, TBWG_TYPE_CHARACTER);
	
	character->characterCode = CHARACTER_DEFAULT;
	character->ID = getID();
	character->position = position;

	character->direction = getfVector(1.0f, 0.0f);

	character->dimension = dimension;

	character->baseStats = defaultStats;
	character->stats = defaultStats;

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
	character->eventerUses = newUses;
	
	character->eventerCount = 1;
	character->eventers = malloc(1*sizeof(struct Eventer));

	character->eventers[0] = getDefaultPunchEventer();

	character->effects = createList();

	character->baseQueue = createQueue();

	struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));
	(*defaultCharTurn) = getBasicCharacterTurn();
	defaultCharTurn->character = character;
	queueAddTurn(&(character->baseQueue), (struct QueueElementHeader*)defaultCharTurn);

	character->headHit = character->bodyHit = character->armHit = character->legHit = characterDefaultHit;

	character->controllerInterface = getstdioControllerInterface();

	character->seeCharacter = defaultSeeCharacter;
	character->canSeen = defaultCanSeen;
	character->seeWorldEvent = defaultSeeWorldEvent;

	return character;
}

float getVisionHardnessFinal(float visionHardness, float distance);


int defaultSeeCharacter(struct Character* observer, struct Character* target)
{
	return target->canSeen(observer, target);
}

int defaultSeeWorldEvent(struct Character* observer, struct WorldEvent* target)
{
	int visionHardnessCheck = observer->stats.visionLevel >= getVisionHardnessFinal(target->visionHardness, getiVectorDistance(observer->position, target->position));
	int speedCheck = observer->stats.visionSpeed >= target->disappearingSpeed;
	return visionHardnessCheck && speedCheck;
}

int defaultCanSeen(struct Character* observer, struct Character* target)
{
	return observer->stats.visionLevel >= target->stats.visionResistence;
}

void destroyCharacter(struct Character* chr)
{
}