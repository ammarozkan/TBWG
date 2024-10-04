#include <TBWG/world.h>
#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <stdlib.h> // malloc



struct World 
createDefaultWorld()
{
	struct List dimensionList = createList();

	struct Dimension* dimension = malloc(sizeof(struct Dimension));
	dimension->ID = getID();
	dimension->characterList = createList();
	dimension->entityList = createList();

	struct DimensionListElement* dimensionListElement = malloc(sizeof(struct DimensionListElement));
	dimensionListElement->dimension = dimension;

	addElement(&dimensionList, (void*)dimensionListElement);



	struct World world = {.dimensionList = dimensionList};

	return world;
}