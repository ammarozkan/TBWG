#ifndef TBWG_ENTITY_H
#define TBWG_ENTITY_H

#include<TBWG/characters.h>

#define ENTITY_DEFAULT 0x01
#define ENTITY_ROCK 0x02

struct Entity;

typedef void (*CollisionFunction) (struct Entity*, struct Character*);

struct Entity {
	TBWGType tbwgType;
	
	digits32 entityCode;
	int x,y;
	float dirx, diry;
	float mass;

	CollisionFunction collisionFunction;
	
	HitterFunction hit;
	
	CanSeen canSeen;
};


#endif /*TBWG_ENTITY_H*/