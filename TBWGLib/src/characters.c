#include <TBWG/characters.h>
#include <stdlib.h> // malloc

struct Stats defaultStats = {0, 0, 0, 0, 0};

//typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);

int characterDefaultHit(void* hitting, struct Character* hitter, struct AttackInfo atk)
{
	TBWGType* tbwgType = (TBWGType*)hitting;
	if (*tbwgType != TBWG_TYPE_CHARACTER) return -1;

	struct Character* hittingCharacter = hitting;
	if(hitter->stats.DEX + atk.additiveStats.DEX > hittingCharacter->stats.DEX || atk.specs & ATTACK_NONDODGEABLE) {
		hittingCharacter->hp -= atk.damage;
		return 1;
	}

	return -1;
}

struct Character* createDefaultCharacter()
{
	struct Character* character = malloc(sizeof(struct Character));

	character->tbwgType = TBWG_TYPE_CHARACTER;
	
	character->characterCode = CHARACTER_DEFAULT;
	character->ID = getID();
	character->x = 0;
	character->y = 0;


	character->baseStats = defaultStats;
	character->stats = defaultStats;

	character->maxhp = 10;
	character->maxe = 5;
	character->maxse = 0;
	character->hp = 10;
	character->maxe = 5;
	character->se = 0;

	character->state = 0;

	character->passivePowerCount = 0;
	character->passivePowers = malloc(0);

	character->eventerCount = 0;
	character->eventers = malloc(0);

	character->effects = createList();

	character->baseQueue = createQueue();

	struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));
	defaultCharTurn->header.type = QUEUE_CHARACTER;
	defaultCharTurn->allowedEventerTypes = 0b11111111111;
	defaultCharTurn->character = character;
	defaultCharTurn->requirements = CHARACTER_REQ_ALIVE | 0b0;
	addTurnToQueue(&(character->baseQueue), (struct QueueElementHeader*)defaultCharTurn);


	character->headHit = character->bodyHit = character->armHit = character->legHit = characterDefaultHit;

	character->controllerInterface = NULL;


	return character;
}



void destroyCharacter(struct Character* chr)
{
}