
void tgfTrickyPunchExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* vt = (struct Eventer*)eventer;

	struct Stats additiveStats = {0};
    additiveStats.DEX = 3;

	if(getiVectorDistance(user->b.position,reqinf.position) > 1.5f) return;

	int r_dmg = tbwgGetRandomed_2(50,10);

	int damage = 4*user->stats.STR;
	if (r_dmg == 0) damage += 0;
	else if(r_dmg == 1) damage += 1*user->stats.STR;

	struct AttackInfo atkInfo = {.additiveStats = additiveStats, .specs = 0, .damageType = DAMAGE_BLUDGEONING, .damage = damage};

	struct Character* c_target = dimensionGetCharacterByPosition(user->b.dimension, reqinf.position);
	if (c_target == NULL) return;

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_TRICKY_PUNCH", 0.5f, 0.5f, user->b.position, WORLDEVENT_VISION));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_MVM_TRICKY_PUNCH", 3.0f, 0.3f, user->b.position, WORLDEVENT_SOUND));

    if (tbwgGetRandomed_2(50,10) == 0)
	    if(!c_target->bodyHit(c_target, (void*)user, atkInfo)) return;
    else {
	    if(!c_target->headHit(c_target, (void*)user, atkInfo)) return;
        else if(r_dmg == 1) {
            chAddEffect(getTGFPuzzledEffect(user, 2), EFFECT_TRIGGER_TYPE_CLOCK, c_target);
            tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_TRICKY_CRITIC", 0.3f, 0.3f, reqinf.position, WORLDEVENT_SOUND));
        }
    }
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_TRICKY_PUNCH", 0.5f, 0.3f, reqinf.position, WORLDEVENT_SOUND));
}

struct Eventer* getTgfTrickyPunch()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = TGF_STREETFIGHTER_TRICKYPUNCH;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_TRICKYPUNCH_NAME);

	evn->baseEnergy = 11;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_FASTCOMBAT;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.fastcombat = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = tgfTrickyPunchExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}