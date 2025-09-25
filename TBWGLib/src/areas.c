#include <TBWG/areas.h>
#include <TBWG/being.h>

void defaultWhenEntered(struct Area* a, struct Being* b)
{
	return;
}

void defaultWhenExited(struct Area* a, struct Being* b)
{
	return;
}

void defaultWhileInside(struct Area* a, struct Being* b)
{
	return;
}


struct Area getDefaultArea(iVector a, iVector b)
{
	struct Area result = {"AREA_DEFAULT",a,b,defaultWhenEntered,defaultWhenExited,defaultWhileInside};
	return result;
}

int isBeingInsideArea(struct Being* c, struct Area* area)
{
	return isPositionInsideArea(c->position, area);
}


int isPositionInsideArea(iVector position, struct Area* area)
{
	return (position.x >= area->a.x && position.x <= area->b.x) && (position.y >= area->a.y && position.y <= area->b.y);
}





#ifdef TBWG_DEBUG
#include <stdio.h>

void debugWhenEntered(struct Area* a, struct Being* b)
{
	printf("DEBUG: Being %u entered area '%s'\n", b->ID, a->name);
	return;
}

void debugWhenExited(struct Area*a, struct Being*b)
{
	printf("DEBUG: Being %u exited area '%s'\n", b->ID, a->name);
	return;
}

void debugWhileInside(struct Area*a, struct Being*b)
{
	printf("DEBUG: Being %u is inside area '%s'\n", b->ID, a->name);
	return;
}

struct Area getDebugArea(iVector a, iVector b)
{
	struct Area result = {"AREA_DEBUG",a,b,debugWhenEntered,debugWhenExited,debugWhileInside};
	return result;
}
#endif /*TBWG_DEBUG*/