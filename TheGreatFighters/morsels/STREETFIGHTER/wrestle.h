
void tgfWrestleExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};
	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = 0, .damageType = DAMAGE_WRESTLE, .damage = 0};
    
    struct TGFGrabbing* grabbing = (struct TGFGrabbing*)beingGetEffectByCode(&(user->b), TGF_TONSOFEFFECTS_GRABBING);
	struct Character* c_target = grabbing->grabbed;
    
	int isgrabbing = tgfGrabbing_isgrabbing(grabbing);
    if (!isgrabbing) return;

	if (c_target == NULL) return;

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_WRESTLE", 0.8f, 0.5f, user->b.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_MVM_WRESTLE", 3.0f, 0.3f, user->b.position, WORLDEVENT_SOUND));

	if(!c_target->bodyHit(c_target, (void*)user, atkInfo)) return;
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WRESTLENING", 0.5f, 0.3f, c_target->b.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_FALL", 0.5f, 0.3f, c_target->b.position, WORLDEVENT_SOUND));

    c_target->state = c_target->state | STATE_ONGROUND;
}

struct Eventer* getTgfWrestle()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_WRESTLE;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_WRESTLE_NAME);

	evn->baseEnergy = 6;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = 0;
	struct EventerUses costs = {0,0,0,0,0};
	costs.fastcombat = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfWrestleExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}
