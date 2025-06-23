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
	uint32_t dimensionCode;
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

int dimensionAddArea(struct Dimension* dim, struct Area* area);
int dimensionAddCharacter(struct Dimension*, struct Character*);

struct Character* dimensionGetCharacterByPosition(struct Dimension* dimension, iVector position);
struct Entity* dimensionGetEntityByPosition(struct Dimension* dimension, iVector position);
struct Being* dimensionGetBeingByPosition(struct Dimension* dimension, iVector position);

struct List dimensionGetAreasOfPosition(struct Dimension* dim, iVector pos);
struct List dimensionGetInAreaCharacters(struct Dimension* dim, iVector a, iVector b);


struct Dimension* createDimension(uint32_t dimensionCode);
struct World createDefaultWorld();
int worldAddCharacter(struct World* world, struct Character* character);
int worldAddDimension(struct World* world, struct Dimension* dimension);
struct Dimension* worldGetDimensionByCode(struct World* w, uint32_t dimensionCode);





#define GET_CHARACTER_LIST_FROM_DIMENSION(dimension) ((struct DimensionListElement*)dimension)->dimension->characterList

#define ITERATE_ALL_CHARACTERS_IN_WORLD(world, charlistelm, dimension) ITERATE(world.dimensionList, dimension) \
	ITERATE(GET_CHARACTER_LIST_FROM_DIMENSION(dimension),charlistelm) 

#endif /*TBWG_WORLD_H*/