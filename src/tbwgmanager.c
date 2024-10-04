#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <stdlib.h> // malloc

struct tbwgdata
tbwgInit()
{
	struct tbwgdata result;
	result.queue = createQueue();
	result.world = createDefaultWorld();
	return result;
}



void tbwgReorder(struct tbwgdata* data)
{
	ITERATE(data->world.dimensionList, dimension) {
		struct List characterList = ((struct DimensionListElement*)dimension)->dimension->characterList;
		ITERATE(characterList,charListElm_pure) {
			struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
			// base queue to data.queue
			mergeBaseQueue(&(data->queue), &(charListElm->character->baseQueue));
		}
	}
}


void tbwgAddCharacter(struct tbwgdata* data, struct Character* character)
{
	struct DimensionListElement* firstDimensionHeader = (struct DimensionListElement*)(data->world.dimensionList.firstelement);

	struct Dimension* firstDimension = firstDimensionHeader->dimension;
	struct List* characterList = &(firstDimension->characterList);
	struct CharacterListElement* charListElm = malloc(sizeof(struct CharacterListElement));
	charListElm->character = character;

	addElement(characterList, (void*)charListElm);
}