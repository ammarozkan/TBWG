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
	iVector position; fVector direction;
	float mass; unsigned int visionHardness;

	float visionAngle; unsigned int visionLevel;

	CollisionFunction collisionFunction;
	
	HitterFunction hit;
	
	CanSeen canSeen;
};


#endif /*TBWG_ENTITY_H*/