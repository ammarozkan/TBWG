#include <TBWG/lists.h>
#include <TBWG/essentials.h>

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
	struct QueueElementHeader header = {.type = QUEUE_REORDER};
};


#define CHARACTER_REQ_ALIVE = 0x01

struct QueueCharacterTurn {
	struct QueueElementHeader header = {.type = QUEUE_CHARACTER};

	digits32 allowedEventerTypes;
	struct Character* character;

	digits32 requirements;
};

struct QueueTimedTurn {
	struct QueueElementHeader header = {.type = QUEUE_TIMED};

	struct TimedEffect* effect;
	void (*executer)(struct World*, struct TimedEffect* effect);
};

struct QueueEntityTurn{
    struct QueueElementHeader header = {.type = QUEUE_ENTITY};

    struct Entity* entity;
    void (*executer)(struct World*, struct Entity* entity);
};

