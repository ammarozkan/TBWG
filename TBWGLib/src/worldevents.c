#include <TBWG/worldevents.h>


struct WorldEvent getDefaultWorldEvent(id_number relative, char* name, float visionHardness, 
	float disappearingSpeed, iVector position, uint32_t streamType)
{
	struct WorldEvent result = {.relative = relative, 
		.name = name, 
		.visionHardness = visionHardness, 
		.disappearingSpeed = disappearingSpeed, 
		.position = position,
		.eventStreamingType = streamType};
	return result;
}