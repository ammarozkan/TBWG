#ifndef TBWG_QUEUE_H
#define TBWG_QUEUE_H

#include <TBWG/lists.h>
#include <TBWG/essentials.h>
#include <TBWG/world.h>

enum QueueElementType {
	QUEUE_CHARACTER, QUEUE_ENTITY, QUEUE_TIMED, QUEUE_REORDER
};

struct Queue {
	struct List queueElements;
};

struct QueueElementHeader {
	struct ListElementHeader listHeader;

	enum QueueElementType type;
};

struct QueueReorderTurn {
	struct QueueElementHeader header;
};


#define CHARACTER_REQ_ALIVE 0x01

struct QueueCharacterTurn {
	struct QueueElementHeader header;

	digits32 allowedEventerTypes;
	struct Character* character;

	digits32 requirements;
};

struct QueueTimedTurn {
	struct QueueElementHeader header;

	struct TimedEffect* effect;
	void (*executer)(struct World*, struct TimedEffect* effect);
};

struct QueueEntityTurn {
    struct QueueElementHeader header;

    struct Entity* entity;
    void (*executer)(struct World*, struct Entity* entity);
};

struct Queue createQueue();
void addTurnToQueue(struct Queue*, struct QueueElementHeader*);

struct QueueElementHeader* queuePop(struct Queue* queue);
void mergeBaseQueue(struct Queue* targetQueue, struct Queue* baseQueue);




#endif /*TBWG_QUEUE_H*/