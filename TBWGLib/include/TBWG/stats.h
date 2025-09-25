#ifndef TBWG_STATS_H
#define TBWG_STATS_H

struct Stats {
	int STR, DEX, CNS, WIS, SCS, SPD;
	int VISRES;
};

struct Stats tbwgSumStats(struct Stats a, struct Stats b);

#endif /*TBWG_STATS_H*/