#ifndef TBWG_WORLDEVENTS_H
#define TBWG_WORLDEVENTS_H

#include <TBWG/maths.h>
#include <TBWG/essentials.h>

struct WorldEvent {
	id_number relative;
	char* name;
	float visionHardness;
	float disappearingSpeed;
	iVector position;
};

struct WorldEvent getDefaultWorldEvent(id_number relative,
	char* name, 
	float visionHardness, 
	float disappearingSpeed, 
	iVector position);


#endif /*TBWG_WORLDEVENTS_H*/
