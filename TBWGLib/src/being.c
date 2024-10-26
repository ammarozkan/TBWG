#include <TBWG/being.h>


int defaultCanSeen(struct Being* observer, struct Being* target)
{
	return observer->eye.level >= target->visionHardness;
}