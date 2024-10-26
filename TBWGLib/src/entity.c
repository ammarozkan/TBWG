#include <TBWG/entity.h>

struct Eye defaultEntityEye = {0.0f, 0.0f, 0.0f};

struct Entity* createDefaultEntity(struct Dimension* dimension, iVector position)
{
	struct Entity* entity = NEW(Entity);
	entity->b.tbwgType = TBWG_TYPE_ENTITY;
	entity->b.ID = getID();
	entity->b.code = ENTITY_DEFAULT;
	entity->b.position = position;
	entity->b.direction = getfVector(0.0f, 1.0f);
	entity->b.dimension = dimension;
	entity->b.baseEye = defaultEntityEye;
	entity->b.eye = defaultEntityEye;
	entity->b.mass = 1.0f;

	entity->b.visionHardness = 0;
	entity->b.baseQueue = createQueue();

	struct QueueEntityTurn* defaultEntityTurn = NEW(QueueEntityTurn);
	(*defaultEntityTurn) = getBasicEntityTurn();
	defaultEntityTurn->entity = entity;
	queueAddTurn(&(entity->b.baseQueue), (struct QueueElementHeader*)defaultEntityTurn);
}

void destroyEntity(struct Entity*);