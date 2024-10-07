#include<TBWG/queue.h>
#include<TBWG/world.h>

struct tbwgdata {
	struct Queue queue;
	struct World world;
};


struct tbwgdata tbwgInit();
void tbwgUse(struct tbwgdata* data);

void tbwgTurn();
void tbwgReorder();

int tbwgAddCharacter(struct Character* character);
struct Dimension* tbwgGetFirstDimension();
struct Dimension* tbwgFindDimensionByPos(unsigned int pos);
struct Dimension* tbwgFindDimensionByID(id_number ID);