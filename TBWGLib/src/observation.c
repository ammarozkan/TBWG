#include <TBWG/observation.h>
#include <TBWG/characters.h>
#include <stdlib.h>
#include <TBWG/maths.h>
#include <TBWG/entity.h>


struct CharacterInformation getCharacterInformation(struct Character* chr)
{
	struct CharacterInformation r;
	r.ID = chr->b.ID;
	r.characterCode = chr->b.code;
	r.position = chr->b.position;
	r.direction = chr->b.direction;
	r.hp = chr->hp;
	return r;
}


struct ObservingInformation Observe(struct Character* as, struct World* world)
{
	struct ObservingInformation obsrv;
	obsrv.selfid = as->b.ID;
	obsrv.characterStats = as->stats;
	obsrv.hp = as->hp;
	obsrv.e = as->e;
	obsrv.se = as->se;

	obsrv.position = as->b.position;

	obsrv.state = as->state;

	//obsrv.effects = as->effects;
	for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT; i += 1)obsrv.effects[i] = as->b.effects[i];

	obsrv.eventerCount = as->eventerCount;
	obsrv.eventers = as->eventers;

	obsrv.characterCount = 0;

	obsrv.charInfos = malloc(sizeof(struct CharacterInformation)*world->characterCount); // max possible character count


	// checking all the characters if they could be seen

	ITERATE_ALL_CHARACTERS_IN_WORLD((*world), charlistelm, dimension) {
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;


		int seeingResult = 0;

		// hmm is it in the vision area?
		seeingResult = seeingResult || isInVisionArea(as->b.direction, as->b.eye.angle, as->b.position, chr->b.position);

		// okay lets look at the ability check
		int seeingCheck = as->seeCharacter(as, chr) && chr->b.canSeen((struct Being*)as, (struct Being*)chr);

		seeingResult = seeingResult && seeingCheck;


		if (seeingResult) goto seeingSuccesfull; // we're succesfull! don't need to further checks.

		ITERATE(as->seeingResources, resourceElement) {
			// we're not succesfull. lets check with vision resource entities.

			TBWGType* resourceType = (TBWGType*)((struct SeeingResourceElement*)resourceElement)->resource;
			if(*resourceType == TBWG_TYPE_CHARACTER) {
				struct Character* sr = (struct Character*)resourceType;

				if(!isInVisionArea(sr->b.direction, sr->b.eye.angle, sr->b.position, chr->b.position)) continue;

				seeingResult = seeingResult || (sr->seeCharacter(sr, chr) && chr->b.canSeen((struct Being*)sr, (struct Being*)chr));
			} else if (*resourceType == TBWG_TYPE_ENTITY) {
				struct Entity* sr = (struct Entity*)resourceType;

				if(!isInVisionArea(sr->b.direction, sr->b.eye.angle, sr->b.position, chr->b.position)) continue;
			}
		}

		if(seeingResult) {
seeingSuccesfull:
			obsrv.charInfos[obsrv.characterCount] = getCharacterInformation(chr);

			obsrv.characterCount += 1;
		}
	}

	// entities not detected here, do that

	void* realEyezRealize = malloc(sizeof(struct CharacterInformation)*obsrv.characterCount);
	tbwgmemcpy(realEyezRealize, obsrv.charInfos, sizeof(struct CharacterInformation)*obsrv.characterCount);
	free(obsrv.charInfos);
	obsrv.charInfos = realEyezRealize;

	return obsrv;
}

struct WorldEventInformation ObserveWorldEventInformation(struct Character* as, struct WorldEvent* evnt)
{
	struct WorldEventInformation info = {as->b.ID, "", evnt->position};
	if(!isInVisionArea(as->b.direction, as->b.eye.angle, as->b.position, evnt->position)) return info;
	if(!as->seeWorldEvent(as, evnt)) return info;
	info.eventName = evnt->name;
	return info;
}