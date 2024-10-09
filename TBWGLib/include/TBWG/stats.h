#ifndef TBWG_STATS_H
#define TBWG_STATS_H

struct Stats {
	int STR, DEX, CNS, WIS, SCS;
	float visionAngle; // in radians (maximum 2.8f for normal people (approximately) )
	float visionLevel;
	float visionResistence;
	float visionSpeed;
};

#endif /*TBWG_STATS_H*/