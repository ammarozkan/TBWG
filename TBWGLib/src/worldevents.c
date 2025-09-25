#include <TBWG/worldevents.h>


struct WorldEvent getDefaultWorldEvent(id_number relative, char* name, float detailLevel, 
	float disappearingSpeed, iVector position, uint32_t streamType)
{
	struct WorldEvent result = {.relative = relative, 
		.name = name, 
		.detailLevel = detailLevel, 
		.disappearingSpeed = disappearingSpeed, 
		.position = position,
		.eventStreamingType = streamType};
	return result;
}