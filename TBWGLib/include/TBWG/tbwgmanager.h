#include<TBWG/queue.h>
#include<TBWG/world.h>
#include<TBWG/worldevents.h>

struct tbwgdata {
	struct Queue queue;
	struct World world;
};


struct tbwgdata* tbwgInit();
void tbwgUse(struct tbwgdata* data);
struct tbwgdata* tbwgGetUsenData();

// unique events
void tbwgTurn();
void tbwgReorder();
void tbwgInterruptTurn(); // will be used when a stopper trap invoked
void tbwgMakeObserveAllCharacters();
void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event);

void tbwgTriggerEffects(unsigned int effectType, void* relativeInformation);
void tbwgAreaWhileInsides();


// setters
int tbwgAddCharacter(struct Character* character);
int tbwgAddArea(struct Dimension* dim, struct Area* area);

// getters
struct World* tbwgGetWorld();
struct Dimension* tbwgGetFirstDimension();
struct Dimension* tbwgFindDimensionByPos(unsigned int pos);
struct Dimension* tbwgFindDimensionByID(id_number ID);

// changers
void tbwgMoveBeing(struct Being*, iVector positionChange);
void tbwgPutBeing(struct Being*, iVector position);


