#ifndef TBWG_WORLD_H
#define TBWG_WORLD_H

#include <TBWG/maths.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/areas.h>

struct Entity;
struct Character;
struct Being;

struct CharacterListElement {
	struct ListElementHeader header;
	struct Character* character;
};

struct EntityListElement {
	struct ListElementHeader header;
	struct Entity* entity;
};

struct BeingListElement {
	struct ListElementHeader header;
	struct Being* being;
};

struct Dimension {
	id_number ID;
	struct List characterList;
	struct List entityList;
	struct List areaList;
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
struct List dimensionGetAreasOfPosition(struct Dimension* dim, iVector pos);
struct List dimensionGetInAreaCharacters(struct Dimension* dim, iVector a, iVector b);

void dimensionAddArea(struct Dimension* dim, struct Area* area);

#define GET_CHARACTER_LIST_FROM_DIMENSION(dimension) ((struct DimensionListElement*)dimension)->dimension->characterList

#define ITERATE_ALL_CHARACTERS_IN_WORLD(world, charlistelm, dimension) ITERATE(world.dimensionList, dimension) \
	ITERATE(GET_CHARACTER_LIST_FROM_DIMENSION(dimension),charlistelm) 

#endif /*TBWG_WORLD_H*/