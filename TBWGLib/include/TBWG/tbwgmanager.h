#include<TBWG/queue.h>
#include<TBWG/world.h>
#include<TBWG/worldevents.h>

struct tbwgdata {
	struct Queue queue;
	struct World world;
};


struct tbwgdata tbwgInit();
void tbwgUse(struct tbwgdata* data);

void tbwgTurn();
void tbwgReorder();
void tbwgMakeObserveAllCharacters();
void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event);

int tbwgAddCharacter(struct Character* character);
struct Dimension* tbwgGetFirstDimension();
struct Dimension* tbwgFindDimensionByPos(unsigned int pos);
struct Dimension* tbwgFindDimensionByID(id_number ID);

void interruptTurn(); // will be used when a stopper trap invoked