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
};

struct World createDefaultWorld();

#endif /*TBWG_WORLD_H*/