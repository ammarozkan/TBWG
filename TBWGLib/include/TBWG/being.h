#ifndef TBWG_BASICBEING_H
#define TBWG_BASICBEING_H

#include <TBWG/essentials.h>
#include <TBWG/world.h>
#include <TBWG/queue.h>
#include <TBWG/observation.h>

struct Being;

typedef void (*CollisionFunction) (struct Being*, struct Being*);
typedef int (*CanSeen)(struct Being* observer, struct Being* target);

struct Being {
	TBWGType tbwgType;
	id_number ID;

	unsigned int code;

	iVector position;
	fVector direction;
	struct Dimension* dimension;

	struct Eye baseEye;
	struct Eye eye;

	float mass; unsigned int visionHardness;

	struct Queue baseQueue;
	struct List effects[EFFECT_TRIGGER_TYPE_COUNT];

	CanSeen canSeen;

	CollisionFunction collisionFunction;
};


int defaultCanSeen(struct Being* observer, struct Being* target);

#endif /*TBWG_BASICBEING_H*/