void tgfGrabExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

    struct TGFGrabbing* grabbing = (struct TGFGrabbing*)beingGetEffectByCode(&(user->b), TGF_TONSOFEFFECTS_GRABBING);
    
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_GRAB", 0.1f, 0.5f, user->b.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_MVM_GRAB", 3.0f, 0.5f, user->b.position, WORLDEVENT_SOUND));

    if(getiVectorDistance(user->b.position, reqinf.position) > 1.5f) return;
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_GRAB_HAND", 0.1f, 0.5f, reqinf.position, WORLDEVENT_VISION));

    struct Character* c_target = dimensionGetCharacterByPosition(user->b.dimension, reqinf.position);

    if (c_target == NULL) return;

	// TRYING TO GRAB
	struct Stats additiveStats = {0};
	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = 0, .damageType = DAMAGE_BLUDGEONING, .damage = 0};

	if(! c_target->bodyHit(c_target, (void*)user, atkInfo)) return;
	
	// GRABBED
	tbwgStreamWorldEvent(c_target->b.dimension, getDefaultWorldEvent(c_target->b.ID, "MVM_GRABBED", 0.1f, 0.3f, reqinf.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(c_target->b.dimension, getDefaultWorldEvent(c_target->b.ID, "SND_GRABBED", 0.1f, 0.3f, reqinf.position, WORLDEVENT_SOUND));
	
	chAddEffect(getTGFGrabbingEffect(user, (struct Being*)c_target, (struct Being*)user), EFFECT_TRIGGER_TYPE_CLOCK, user);
}

struct Eventer* getTgfGrab()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_GRAB;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_GRAB_NAME);

	evn->baseEnergy = 5;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.fastcombat = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfGrabExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}