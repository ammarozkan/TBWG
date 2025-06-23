void tgfGetUpExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;
    
    user->state = user->state & (~STATE_ONGROUND);
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_GETUP", 0.7f, 0.5f, user->b.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_GETUP", 0.5f, 0.5f, user->b.position, WORLDEVENT_VISION));
}

struct Eventer* getTgfGetUp()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_GETUP;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_GETUP_NAME);

	evn->baseEnergy = 2;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_RESTING;
	evn->required_informations = 0;
	struct EventerUses costs = {0,0,0,0,0};
    costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady_chrAwake;

	evn->executer = tgfGetUpExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}