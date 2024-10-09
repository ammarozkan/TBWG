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



	struct World world = {.dimensionList = dimensionList, .characterCount = 0};

	return world;
}

struct Character*
dimensionGetCharacterByPosition(struct Dimension* dimension, int x, int y)
{
	ITERATE(dimension->characterList, charListElm_pure) {
		struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
		struct Character* chr = charListElm->character;

		if(chr->position.x == x && chr->position.y == y) return chr;
	}

	return NULL;
}

int
addCharacterToWorld(struct World* world, struct Character* character)
{
	struct Dimension* dimension = character->dimension;
	if(dimension == NULL) return 0;

	struct List* characterList = &(dimension->characterList);
	struct CharacterListElement charListElm = {.character = character};

	addElement(characterList, (void*)&charListElm, sizeof(struct CharacterListElement));
	world->characterCount += 1;
	return 1;
}