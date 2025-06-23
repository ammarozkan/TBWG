#ifndef TBWG_WORLDEVENTS_H
#define TBWG_WORLDEVENTS_H

#include <TBWG/maths.h>
#include <TBWG/essentials.h>

#define WORLDEVENT_SOUND (1<<0)
#define WORLDEVENT_VISION (1<<1)


struct WorldEvent {
	id_number relative;
	char* name;
	float detailLevel;
	float disappearingSpeed;
	uint32_t eventStreamingType; // how can I see the event?
	iVector position;
};

struct WorldEvent getDefaultWorldEvent(id_number relative,
	char* name, 
	float detailLevel, 
	float disappearingSpeed, 
	iVector position,
	uint32_t eventStreamingType);


#endif /*TBWG_WORLDEVENTS_H*/
