#include <TBWG/observation.h>
#include <TBWG/characters.h>
#include <stdlib.h>


struct ObservingInformation Observe(struct Character* as, struct World* world)
{
	struct ObservingInformation obsrv;
	obsrv.selfid = as->ID;
	obsrv.characterStats = as->stats;
	obsrv.hp = as->hp;
	obsrv.e = as->e;
	obsrv.se = as->se;

	obsrv.state = as->state;

	obsrv.effects = as->effects;

	obsrv.eventerCount = as->eventerCount;
	obsrv.eventers = as->eventers;

	obsrv.characterCount = 0;
	obsrv.charInfos = malloc(sizeof(struct CharacterInformation)*64);

	ITERATE_ALL_CHARACTERS_IN_WORLD((*world), charlistelm, dimension) {
		struct Character* chr = ((struct CharacterListElement*)charlistelm)->character;
	}
}