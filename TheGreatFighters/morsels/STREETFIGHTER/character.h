

struct Character* tgfGetStreetFighter(struct Dimension* dimension, iVector position)
{
	struct Character* character = malloc(sizeof(struct Character));

	character->b.tbwgType = TBWG_TYPE_CHARACTER;
	
	character->b.code = TGF_STREETFIGHTER_CHARACTERCODE;
	character->b.ID = getID();
	character->b.position = position;

	character->b.direction = getfVector(1.0f, 0.0f);

	character->b.dimension = dimension;

	character->b.collisionFunction = beingDefaultOneWayCollision;

	character->baseStats = tgfDefaultStats;
    character->baseStats.STR += 2;
    character->baseStats.CNS += 4;
    character->baseStats.DEX += 3;
	character->stats = character->baseStats;

	character->b.visionHardness = 0;

	struct Eye eye = {1.8f, 1.0f, 0.1f, 1.0f, 0.1f};
	character->b.baseEye = eye;
	character->b.eye = eye;

	iValue hp = getHPByCONS(character->stats.CNS);
	iValue e = {40,50};
	iValue se = {0,0};

	character->hp = hp;
	character->e = e;
	character->se = se;

	character->state = 0;

	character->passivePowerCount = 0;
	character->passivePowers = malloc(0);

	struct EventerUses newUses = {0,0,0,0,0};
	character->eventerSpendings = newUses;
	
	character->eventerCount = 12;
	character->eventers = malloc(12*sizeof(struct Eventer));

	character->eventers[0] = getTgfPunch();
	character->eventers[1] = getTgfWalk();
	character->eventers[2] = getTgfLook();
	character->eventers[3] = getTgfRest();
	character->eventers[4] = getTgfPush();
	character->eventers[5] = getTgfGrab();
	character->eventers[6] = getTgfGrabWalk();
	character->eventers[7] = getTgfSilentWalk();
	character->eventers[8] = getTgfTrickyPunch();
	character->eventers[9] = getTgfGetUp();
	character->eventers[10] = getTgfGetDown();
	character->eventers[11] = getTgfWrestle();

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