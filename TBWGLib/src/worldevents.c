#include <TBWG/worldevents.h>


struct WorldEvent getDefaultWorldEvent(char* name, float visionHardness, float disappearingSpeed, iVector position)
{
	struct WorldEvent result = {name, visionHardness, disappearingSpeed, position};
	return result;
}