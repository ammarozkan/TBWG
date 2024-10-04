#include<TBWG/queue.h>
#include<TBWG/world.h>

struct tbwgdata {
	struct Queue queue;
	struct World world;
};


struct tbwgdata tbwgInit();

void tbwgReorder(struct tbwgdata* data);

void tbwgAddCharacter(struct tbwgdata* data, struct Character* character);