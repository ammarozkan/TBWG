#ifndef TBWG_WORLD_H
#define TBWG_WORLD_H

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
struct Entity;
struct Character;

struct CharacterListElement {
	struct ListElementHeader header;
	struct Character* character;
};

struct EntityListElement {
	struct ListElementHeader header;
	struct Entity* entity;
};

struct Dimension {
	id_number ID;
	struct List characterList;
	struct List entityList;
};

struct DimensionListElement {
	struct ListElementHeader header;
	struct Dimension* dimension;
};

struct World {
	struct List dimensionList;

	unsigned int characterCount;
};

struct World createDefaultWorld();
int addCharacterToWorld(struct World* world, struct Character* character);

struct Character* dimensionGetCharacterByPosition(struct Dimension* dimension, int x, int y);

#define GET_CHARACTER_LIST_FROM_DIMENSION(dimension) ((struct DimensionListElement*)dimension)->dimension->characterList

#define ITERATE_ALL_CHARACTERS_IN_WORLD(world, charlistelm, dimension) ITERATE(world.dimensionList, dimension) \
	ITERATE(GET_CHARACTER_LIST_FROM_DIMENSION(dimension),charlistelm) 

#endif /*TBWG_WORLD_H*/