#include <TBWG/observation.h>
#include <TBWG/characters.h>
#include <stdlib.h>
#include <TBWG/maths.h>
#include <TBWG/entity.h>


struct CharacterInformation getCharacterInformation(struct Character* chr)
{
	struct CharacterInformation r;
	r.ID = chr->ID;
	r.characterCode = chr->characterCode;
	r.position = chr->position;
	r.direction = chr->direction;
	r.hp = chr->hp;
	return r;
}


struct ObservingInformation Observe(struct Character* as, struct World* world)
{
	struct ObservingInformation obsrv;
	obsrv.selfid = as->ID;
	obsrv.characterStats = as->stats;
	obsrv.hp = as->hp;
	obsrv.e = as->e;
	obsrv.se = as->se;

	obsrv.position = as->position;

	obsrv.state = as->state;

	obsrv.effects = as->effects;

	obsrv.eventerCount = as->eventerCount;
	obsrv.eventers = as->eventers;

	obsrv.characterCount = 0;

	obsrv.charInfos = malloc(sizeof(struct CharacterInformation)*world->characterCount); // max possible character count


	// checking all the characters if they could be seen

	ITERATE_ALL_CHARACTERS_IN_WORLD((*world), charlistelm, dimension) {
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;


		int seeingResult = 0;

		// hmm is it in the vision area?
		seeingResult = seeingResult || isInVisionArea(as->direction, as->stats.visionAngle, as->position, chr->position);

		// okay lets look at the ability check
		int seeingCheck = as->seeCharacter(as, chr) && chr->canSeen(as, chr);

		seeingResult = seeingResult && seeingCheck;


		if (seeingResult) goto seeingSuccesfull; // we're succesfull! don't need to further checks.

		ITERATE(as->seeingResources, resourceElement) {
			// we're not succesfull. lets check with vision resource entities.

			TBWGType* resourceType = (TBWGType*)((struct SeeingResourceElement*)resourceElement)->resource;
			if(*resourceType == TBWG_TYPE_CHARACTER) {
				struct Character* sr = (struct Character*)resourceType;

				if(!isInVisionArea(sr->direction, sr->stats.visionAngle, sr->position, chr->position)) continue;

				seeingResult = seeingResult || (sr->seeCharacter(sr, chr) && chr->canSeen(sr, chr));
			} else if (*resourceType == TBWG_TYPE_ENTITY) {
				struct Entity* sr = (struct Entity*)resourceType;

				if(!isInVisionArea(sr->direction, sr->visionAngle, sr->position, chr->position)) continue;
			}
		}

		if(seeingResult) {
seeingSuccesfull:
			obsrv.charInfos[obsrv.characterCount] = getCharacterInformation(chr);

			obsrv.characterCount += 1;
		}
	}

	void* realEyezRealize = malloc(sizeof(struct CharacterInformation)*obsrv.characterCount);
	tbwgmemcpy(realEyezRealize, obsrv.charInfos, sizeof(struct CharacterInformation)*obsrv.characterCount);
	free(obsrv.charInfos);
	obsrv.charInfos = realEyezRealize;

	return obsrv;
}

struct WorldEventInformation ObserveWorldEventInformation(struct Character* as, struct WorldEvent* evnt)
{
	struct WorldEventInformation info = {as->ID, "", evnt->position};
	if(!as->seeWorldEvent(as, evnt)) return info;
	info.eventName = evnt->name;
	return info;
}