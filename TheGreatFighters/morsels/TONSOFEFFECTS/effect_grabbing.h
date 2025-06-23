
struct TGFGrabbing {
    struct Effect effect;
    iVector grabbedPosition;
    iVector from;
    struct Being* grabbed;
    struct Being* grabber;
};


int tgfGrabbing_isgrabbing(struct TGFGrabbing* f)
{
    if(f->grabbed == NULL) 
        goto endgrabbing;

    if(getiVectorDistance(f->grabbedPosition,f->grabbed->position) != 0) 
        goto endgrabbing;

    if(getiVectorDistance(f->from, f->grabber->position) != 0)
        goto endgrabbing;

    goto continuegrabbing;


endgrabbing:
    f->effect.willberemoved = 1;
    if (f->grabbed != NULL)
    	tbwgStreamWorldEvent(((struct Being*)f->effect.effector)->dimension, getDefaultWorldEvent(f->grabber->ID, "SND_BREAK_GRABBING", 0.7f, 0.3f, f->grabbedPosition, WORLDEVENT_SOUND));
    f->grabbed = NULL;
    return 0;
continuegrabbing:
    return 1;
}

void tgfGrabbingExecuter(void* effectptr, struct World* w, struct Character* entity, void* relativeInformation)
{
	struct TGFGrabbing* f = (struct TGFGrabbing*)effectptr;

    tgfGrabbing_isgrabbing(f);
}

struct Effect* getTGFGrabbingEffect(void* effector, struct Being* grabbed, struct Being* grabber)
{
	struct TGFGrabbing* f = GET_SIZED_STRUCT(TGFGrabbing);
	f->effect.ID = getID();
	f->effect.code = TGF_TONSOFEFFECTS_GRABBING;

	TBWGType effectorType = TBWG_TYPE_UNKNOWN;
	if(effector != NULL) effectorType = *((TBWGType*)effector);

	f->effect.effectorType = effectorType;
	f->effect.effector = effector;

	f->effect.time = -1;
	f->effect.willberemoved = 0;

	f->effect.effectSpecs = 0;

	struct Stats s = {0, 0, 0, 0, 0, 0, 0};
	f->effect.givenStats = s;

	f->effect.executer = tgfGrabbingExecuter;

    f->grabbedPosition = grabbed->position;
    f->from = ((struct Being*)effector)->position;
    f->grabbed = grabbed;
    f->grabber = grabber;
	return (struct Effect*)f;
}