#ifndef TBWG_AREAS_H
#define TBWG_AREAS_H

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/maths.h>

struct Area;
struct Being;

typedef void (*AreaEventerFunction)(struct Area*, struct Being*);

struct Area {
	char* name;

	iVector a, b; // b > a!

	AreaEventerFunction whenEntered, whenExited, whileInside;
};

struct AreaListElement {
	struct ListElementHeader header;
	struct Area* area;
};

void defaultWhenEntered(struct Area*, struct Being*);
void defaultWhenExited(struct Area*, struct Being*);
void defaultWhileInside(struct Area*, struct Being*);

struct Area getDefaultArea(iVector a, iVector b);
int isPositionInsideArea(iVector pos, struct Area*);
int isBeingInsideArea(struct Being*, struct Area*);


#ifdef TBWG_DEBUG
struct Area getDebugArea(iVector a, iVector b);
#endif /*TBWG_DEBUG*/

#endif /*TBWG_AREAS_H*/