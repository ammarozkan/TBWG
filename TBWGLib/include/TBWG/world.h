#include<TBWG/lists.h>
#include<TBWG/entity.h>
#include<TBWG/characters.h>

struct CharacterListElement {
	struct ListElementHeader header;
	struct Character* character;
};

struct EntityListElement {
	struct ListElementHeader header;
	struct Entity* entity;
};

struct Dimension {
	struct List characterList;
	struct List entityList;
};


struct World {
	struct List dimensionList;
};