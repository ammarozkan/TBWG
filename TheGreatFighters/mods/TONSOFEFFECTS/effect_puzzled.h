
struct TGFPuzzled {
    struct Effect effect;
};

struct Effect* getTGFPuzzledEffect(void* effector, int time)
{
	struct TGFPuzzled* f = GET_SIZED_STRUCT(TGFPuzzled);
	f->effect.ID = getID();
	f->effect.code = TGF_TONSOFEFFECTS_PUZZLED;

	TBWGType effectorType = TBWG_TYPE_UNKNOWN;
	if(effector != NULL) effectorType = *((TBWGType*)effector);

	f->effect.effectorType = effectorType;
	f->effect.effector = effector;

	f->effect.time = time;
	f->effect.willberemoved = 0;

	f->effect.effectSpecs = 0;

	struct Stats s = {-1, -2, 0, 0, -1, -1, 0};
	f->effect.givenStats = s;

	f->effect.executer = defaultEffectExecuter;
	return (struct Effect*)f;
}