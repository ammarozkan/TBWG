#include <TBWG/entity.h>

struct Eye defaultEntityEye = {0.0f, 0.0f, 0.0f};

struct Eye getDefaultEntityEye()
{
	return defaultEntityEye;
}

struct Entity* createDefaultEntity(struct Dimension* dimension, iVector position)
{
	struct Entity* entity = NEW(Entity);
	entity->b.tbwgType = TBWG_TYPE_ENTITY;
	entity->b.ID = getID();
	entity->b.code = ENTITY_DEFAULT;
	entity->b.position = position;
	entity->b.direction = getfVector(0.0f, 1.0f);
	entity->b.dimension = dimension;
	entity->b.baseEye = getDefaultEntityEye();
	entity->b.eye = getDefaultEntityEye();
	entity->b.mass = 1.0f;

	entity->b.visionHardness = 0;
	entity->b.baseQueue = createQueue();

	entity->b.collisionFunction = beingDefaultOneWayCollision;

	struct QueueEntityTurn* defaultEntityTurn = NEW(QueueEntityTurn);
	(*defaultEntityTurn) = getBasicEntityTurn(entity);
	queueAddTurn(&(entity->b.baseQueue), (struct QueueElementHeader*)defaultEntityTurn);

	entity->eventer = defaultEntityEventer;
}

void destroyEntity(struct Entity* entity)
{
	// destroying thing here
	destroyQueue((struct Queue*)&(entity->b.baseQueue));
	free(entity);
}

void defaultEntityEventer(struct Entity* e)
{
	return;
}