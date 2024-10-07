#include <TBWG/world.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/characters.h>
#include <stdlib.h> // malloc



struct World 
createDefaultWorld()
{
	struct List dimensionList = createList();

	struct Dimension* dimension = malloc(sizeof(struct Dimension));
	dimension->ID = getID();
	dimension->characterList = createList();
	dimension->entityList = createList();

	struct DimensionListElement dimensionListElement = {.dimension = dimension};

	addElement(&dimensionList, (void*)&dimensionListElement, sizeof(struct DimensionListElement));



	struct World world = {.dimensionList = dimensionList};

	return world;
}

struct Character*
dimensionGetCharacterByPosition(struct Dimension* dimension, int x, int y)
{
	ITERATE(dimension->characterList, charListElm_pure) {
		struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
		struct Character* chr = charListElm->character;

		if(chr->x == x && chr->y == y) return chr;
	}

	return NULL;
}