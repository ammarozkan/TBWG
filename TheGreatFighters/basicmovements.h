
void defaultLookEventerExecuter(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool)
{
	struct WorldEvent lookMove = getDefaultWorldEvent(user->b.ID, "DEFAULT_LOOK", 0.0f, 0.5f, user->b.position, WORLDEVENT_VISION);
	tbwgStreamWorldEvent(user->b.dimension, lookMove);

	user->b.direction = reqinf.direction;
}

struct Eventer* getDefaultLookEventer()
{
	struct Eventer* evn = GET_SIZED_STRUCT(Eventer);
	evn->eventerCode = EVENTER_DEFAULT;
	evn->ID = getID();
	getName(evn->name, "DEFAULT_LOOK");

	evn->baseEnergy = 1;
	evn->baseSpellEnergy = 0;

	evn->eventer_type = EVENTER_TYPE_CLASSIC;
	evn->required_informations = EVENTER_REQUIRED_INFORMATION_DIRECTION;
	struct EventerUses costs = {0,0,0,0,0};
	costs.movement = 1;
	evn->costs = costs;

	evn->setReady = defaultSetEventerReady;

	evn->executer = defaultLookEventerExecuter;
	evn->notChoosed = defaultEventerNotChoosed;
	evn->canExecutedNow = defaultEventerCanExecutedNow;
	return evn;
}


struct Character* createDefaultTheGreatCharacter(struct Dimension* dimension, iVector position)
{
    struct Character* c = createDefaultCharacter(dimension, position);
    free(c->eventers);
    c->eventerCount = 3;
    c->eventers = malloc(3*sizeof(struct Eventer));
    
	c->eventers[0] = getDefaultPunchEventer();
	c->eventers[1] = getDefaultWalkEventer();
    c->eventers[2] = getDefaultLookEventer();
	c->b.code = 32;
    return c;
}