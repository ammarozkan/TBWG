void tgfGrabWalkExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

    struct TGFGrabbing* grabbing = (struct TGFGrabbing*)beingGetEffectByCode(&(user->b), TGF_TONSOFEFFECTS_GRABBING);

    iVector mvm = connectiVectors(user->b.position, reqinf.position);

	int isgrabbing = tgfGrabbing_isgrabbing(grabbing);

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 0.7f, 0.5f, user->b.position, WORLDEVENT_SOUND));
	if(!tbwgMoveBeing((struct Being*)user, mvm)) return;
	
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 0.7f, 0.5f, reqinf.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_WALK", 0.7f, 0.5f, reqinf.position, WORLDEVENT_VISION));

	if(!isgrabbing) return;

    struct Being* grabbed = grabbing->grabbed;

	tbwgStreamWorldEvent(grabbed->dimension, getDefaultWorldEvent(grabbed->ID, "SND_WALK", 0.7f, 0.5f, grabbed->position, WORLDEVENT_SOUND));
	if(!tbwgMoveBeing(grabbed, mvm)) return;

	grabbing->grabbedPosition = grabbed->position;
	grabbing->from = user->b.position;
	
	tbwgStreamWorldEvent(grabbed->dimension, getDefaultWorldEvent(grabbed->ID, "SND_WALK", 0.7f, 0.5f, reqinf.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(grabbed->dimension, getDefaultWorldEvent(grabbed->ID, "MVM_WALK", 0.7f, 0.5f, reqinf.position, WORLDEVENT_VISION));

	
}

struct Eventer* getTgfGrabWalk()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_GRABWALK;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_GRABWALK_NAME);

	evn->baseEnergy = 5;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
    costs.movement = 1;
	costs.fastcombat = 0;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfGrabWalkExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = tgfCanWalk;
	return evn;
}