#ifndef TBWG_WORLDEVENTS_H
#define TBWG_WORLDEVENTS_H

#include <TBWG/maths.h>

struct WorldEvent {
	char* name;
	float visionHardness;
	float disappearingSpeed;
	iVector position;
};

struct WorldEvent getDefaultWorldEvent(char* name, 
	float visionHardness, 
	float disappearingSpeed, 
	iVector position);


#endif /*TBWG_WORLDEVENTS_H*/
