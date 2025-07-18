#include <TBWG/world.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/characters.h>
#include <TBWG/entity.h>
#include <stdlib.h> // malloc


struct Dimension*
createDimension(uint32_t dimensionCode)
{
	struct Dimension* dimension = malloc(sizeof(struct Dimension));
	
	dimension->ID = getID();
	dimension->dimensionCode = dimensionCode;
	dimension->characterList = createList();
	dimension->entityList = createList();
	dimension->areaList = createList();
	return dimension;
}

struct World 
createDefaultWorld()
{
	struct List dimensionList = createList();

	struct World world = {.dimensionList = dimensionList, .characterCount = 0};
	worldAddDimension(&world, createDimension(0x0));

	return world;
}

struct Character*
dimensionGetCharacterByPosition(struct Dimension* dimension, iVector position)
{
	ITERATE(dimension->characterList, charListElm_pure) {
		struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;
		struct Character* chr = charListElm->character;

		if(chr->b.position.x == position.x && chr->b.position.y == position.y) return chr;
	}

	return NULL;
}


struct Entity* dimensionGetEntityByPosition(struct Dimension* dimension, iVector position)
{
	ITERATE(dimension->entityList, entityListElm_pure) {
		struct EntityListElement* entityListElm = (struct EntityListElement*)entityListElm_pure;
		struct Entity* ent = entityListElm->entity;

		if(ent->b.position.x == position.x && ent->b.position.y == position.y) return ent;
	}

	return NULL;
}


struct Being* dimensionGetBeingByPosition(struct Dimension* dimension, iVector position)
{
	struct Character* chr = dimensionGetCharacterByPosition(dimension, position);
	if (chr != NULL) return (struct Being*)chr;
	return (struct Being*)dimensionGetEntityByPosition(dimension, position);
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

int
dimensionAddEntity(struct Dimension* dim, struct Entity* e)
{
	struct List areas = dimensionGetAreasOfPosition(dim, e->b.position);
	ITERATE(areas, area_elm) {
		struct Area* area = ((struct AreaListElement*)area_elm)->area;
		area->whenEntered(area, (struct Being*)e);
	}
	freeListHeaders(areas);

	struct EntityListElement elm = {.entity = e};
	addElement(&(dim->entityList), (void*)&elm, sizeof(elm));

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

int
worldAddEntity(struct World* world, struct Entity* entity)
{
	struct Dimension* dimension = entity->b.dimension;
	if(dimension == NULL) return 0;

	dimensionAddEntity(dimension, entity);
	return 1;
}

int
worldAddDimension(struct World* world, struct Dimension* dimension)
{
	struct DimensionListElement dimensionListElement = {.dimension = dimension};
	addElement(&(world->dimensionList), (void*)&dimensionListElement, sizeof(struct DimensionListElement));
	return 1;
}


struct Dimension* 
worldGetDimensionByCode(struct World* w, uint32_t dimensionCode)
{
	ITERATE(w->dimensionList, dimension) {
		struct Dimension* dim = ((struct DimensionListElement*)dimension)->dimension;
		if (dim->dimensionCode == dimensionCode) return dim;
	}
	return NULL;
}