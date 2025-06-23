struct TgfSilentWalkEventer {
	struct Eventer evn;
	int ready;
};


void tgfSilentWalkNotChoosed(void* eventer, struct World*, struct Character*)
{
	struct TgfSilentWalkEventer* swe = (struct TgfSilentWalkEventer*)eventer;
	swe->ready = 0;
}

int tgfSilentWalkSetReady(struct Eventer* e, struct Character* c, struct World* w)
{
	struct TgfSilentWalkEventer* swe = (struct TgfSilentWalkEventer*)e;
	if(swe->ready == 0) {
		e->required_informations = 0;
	} else if (swe->ready == 1) {
		e->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	}
	return defaultSetEventerReady(e, c, w);
}



int tgfCanSilentWalk(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* evn = (struct Eventer*) eventer;
	struct TgfSilentWalkEventer* swe = (struct TgfSilentWalkEventer*)eventer;
	if(swe->ready == 0) return 1;

	iVector pos = user->b.position;
	iVector target = reqinf.position;

	float maxWalk = ((float)(user->stats.SPD) * 0.1f);

	return getiVectorDistance(pos,target)*0.5f <= maxWalk;
}

void tgfSilentWalkExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct Eventer* evn = (struct Eventer*) eventer;
	struct TgfSilentWalkEventer* swe = (struct TgfSilentWalkEventer*)evn;

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_SILENTWALK", 0.7f, 0.5f, user->b.position, WORLDEVENT_VISION));

	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.2f, 0.5f, user->b.position, WORLDEVENT_SOUND));

	if(swe->ready == 0) {swe->ready = 1; return;}
	if(swe->ready == 1) {swe->ready = 0;}
	if(!tbwgPutBeing((struct Being*)user, reqinf.position)) return;
	
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "SND_WALK", 1.2f, 0.5f, reqinf.position, WORLDEVENT_SOUND));
	tbwgStreamWorldEvent(user->b.dimension, getDefaultWorldEvent(user->b.ID, "MVM_SILENTWALK", 0.7f, 0.5f, reqinf.position, WORLDEVENT_VISION));
}

struct Eventer* getTgfSilentWalk()
{
	struct Eventer* evn = GET_SIZED_STRUCT(TgfSilentWalkEventer);
	evn->eventerCode = TGF_STREETFIGHTER_SILENTWALK;
	evn->ID = getID();
	getName(evn->name, TGF_STREETFIGHTER_SILENTWALK_NAME);

	evn->baseEnergy = 1;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_CLASSIC;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_POSITION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.movement = 1;
	evn->costs = costs;

	evn->setReady = tgfSilentWalkSetReady;
	evn->executer = tgfSilentWalkExecuter;
	evn->canExecutedNow = tgfCanSilentWalk;
	evn->notChoosed = tgfSilentWalkNotChoosed;

	struct TgfSilentWalkEventer* swe = (struct TgfSilentWalkEventer*)evn;
	swe->ready;
	return evn;
}