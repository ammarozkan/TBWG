void tgfPushExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};

	if(getiVectorDistance(user->b.position,reqinf.position));

    iVector mvm = connectiVectors(user->b.position, reqinf.position);

	int r_dmg = tbwgGetRandomed_2(60,10);

	int damage = 3*user->stats.STR;
	if (r_dmg == 0) damage += 0;
	else if(r_dmg == 1) damage += 1*user->stats.STR;

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = 0, .damageType = DAMAGE_BLUDGEONING, .damage = damage};

	struct Character* c_target = dimensionGetCharacterByPosition(user->b.dimension, reqinf.position.x, reqinf.position.y);

    tbwgPutBeing((struct Being*)user, addiVectors(user->b.position,mvm));

    if (c_target == NULL) return;

    chAddEffect(getTGFPuzzledEffect(user, 10), EFFECT_TRIGGER_TYPE_CLOCK, c_target);
    tbwgPutBeing((struct Being*)c_target, addiVectors(c_target->b.position,mvm));

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "ACT_PUSHED", 1.0f, 0.5f, c_target->b.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_PUSH", 1.0f, 0.5f, user->b.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_PUSH", 1.0f, 0.5f, reqinf.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.0f, 0.5f, user->b.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.0f, 0.5f, c_target->b.position, WORLDEVENT_SOUND));
	if(c_target->bodyHit(c_target, (void*)user, atkInfo)) ;
}

struct Eventer* getTgfPush()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_PUSH;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_PUSH_NAME);

	evn->baseEnergy = 8;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.fastcombat = 1;
    costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfPushExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = tgfCanWalk;
	return evn;
}