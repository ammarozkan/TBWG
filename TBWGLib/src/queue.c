#include<TBWG/queue.h>

struct Queue 
createQueue()
{
	struct Queue queue;

	struct List queueElements = createList();
	queue.queueElements = queueElements;

	return queue;
}

void
destroyQueue(struct Queue* queue)
{
	decolonizeList((struct List*)&(queue->queueElements));
}

void
queueAddTurn(struct Queue* queue, struct QueueElementHeader* turn)
{
	unsigned long int turnSize = 0;
	if(turn->type == QUEUE_CHARACTER) turnSize = sizeof(struct QueueCharacterTurn);
	else if(turn->type == QUEUE_ENTITY) turnSize = sizeof(struct QueueEntityTurn);
	else if(turn->type == QUEUE_TIMED) turnSize = sizeof(struct QueueTimedTurn);
	else if(turn->type == QUEUE_REORDER) turnSize = sizeof(struct QueueReorderTurn);
	addElement(&(queue->queueElements), (void*)turn, turnSize);
}

struct QueueElementHeader* 
queuePop(struct Queue* queue)
{
	struct List* list = &(queue->queueElements);

	struct ListElementHeader* result = popElement(list, list->firstelement);

	return (struct QueueElementHeader*)result;
}

int
queueIsEmpty(struct Queue* queue)
{
	return listIsEmpty(&(queue->queueElements));
}



void mergeBaseQueue(struct Queue* targetQueue, struct Queue* baseQueue)
{
	ITERATE(baseQueue->queueElements, turn) {
		queueAddTurn(targetQueue, (struct QueueElementHeader*)turn);
	}
}




void defaultCharacterTurnInvoke(struct QueueCharacterTurn* t)
{
	return;
}


struct QueueCharacterTurn getBasicCharacterTurn()
{
	struct QueueCharacterTurn r;
	r.header.listHeader.prior = NULL;
	r.header.listHeader.next = NULL;
	r.header.type = QUEUE_CHARACTER;
	r.allowedEventerTypes = 0b1111111111;

	struct EventerUses evnu = {1,0,0,0,0};
	r.gainingUses = evnu;
	r.character = NULL;
	r.whenInvoked = defaultCharacterTurnInvoke;
	r.requirements = 0 | CHARACTER_REQ_ALIVE;
	return r;
}


struct QueueEntityTurn getBasicEntityTurn(struct Entity* entity)
{
	struct QueueEntityTurn r;
	r.header.listHeader.prior = NULL;
	r.header.listHeader.next = NULL;
	r.header.type = QUEUE_ENTITY;
	r.entity = entity;
	r.whenInvoked = defaultEntityTurnInvoke;
	return r;
}

void defaultEntityTurnInvoke(struct QueueEntityTurn* t)
{
	return;
}

