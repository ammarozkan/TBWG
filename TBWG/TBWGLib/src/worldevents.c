#include <TBWG/worldevents.h>


struct WorldEvent getDefaultWorldEvent(id_number relative, char* name, float visionHardness, float disappearingSpeed, iVector position)
{
	struct WorldEvent result = {relative, name, visionHardness, disappearingSpeed, position};
	return result;
}