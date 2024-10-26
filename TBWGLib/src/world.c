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

		if(chr->b.position.x == x && chr->b.position.y == y) return chr;
	}

	return NULL;
}

int 
dimensionAddArea(struct Dimension* dim, struct Area* area)
{
	struct List effectedCharacters = dimensionGetInAreaCharacters(dim, area->a, area->b);
	ITERATE(effectedCharacters, ch_elm) {
		struct Character* ch = ((struct CharacterListElement*)ch_elm)->character;
		area->whenEntered(area, (struct Being*)ch);
	}
	freeListHeaders(effectedCharacters);

	struct AreaListElement elm = {.area = area};
	addElement(&(dim->areaList), &elm, sizeof(elm));

	return 1;
}

int
dimensionAddCharacter(struct Dimension* dim, struct Character* c)
{
	struct List areas = dimensionGetAreasOfPosition(dim, c->b.position);
	ITERATE(areas, area_elm) {
		struct Area* area = ((struct AreaListElement*)area_elm)->area;
		area->whenEntered(area, (struct Being*)c);
	}
	freeListHeaders(areas);

	struct CharacterListElement elm = {.character = c};
	addElement(&(dim->characterList), (void*)&elm, sizeof(elm));

	return 1;
}

struct List dimensionGetAreasOfPosition(struct Dimension* dim, iVector pos)
{
	struct List result = createList();
	ITERATE(dim->areaList, area_elm) {
		struct Area* area = ((struct AreaListElement*)area_elm)->area;
		struct AreaListElement alef = {.area = area};
		if(isPositionInsideArea(pos, area)) addElement(&result, (void*)&alef, sizeof(alef));
	}

	return result;
}


struct List dimensionGetInAreaCharacters(struct Dimension* dim, iVector a, iVector b)
{
	struct List result = createList();
	ITERATE(dim->characterList, ch_elm) {
		struct Character* ch = ((struct CharacterListElement*)ch_elm)->character;
		struct CharacterListElement alef = {.character = ch};
		if(isPositionInside(ch->b.position, a, b)) addElement(&result, (void*)&alef, sizeof(alef));
	}

	return result;
}

int
worldAddCharacter(struct World* world, struct Character* character)
{
	struct Dimension* dimension = character->b.dimension;
	if(dimension == NULL) return 0;

	dimensionAddCharacter(dimension, character);

	world->characterCount += 1;
	return 1;
}