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
addTurnToQueue(struct Queue* queue, struct QueueElementHeader* turn)
{
	addElement(&(queue->queueElements), (void*)turn);
}

struct QueueElementHeader* 
queuePop(struct Queue* queue)
{
	struct List* list = &(queue->queueElements);

	struct ListElementHeader* result = popElement(list, list->firstelement);

	return (struct QueueElementHeader*)result;
}


void mergeBaseQueue(struct Queue* targetQueue, struct Queue* baseQueue)
{
	ITERATE(baseQueue->queueElements, turn) {
		addElement(&(targetQueue->queueElements), (void*)turn);
	}
}

