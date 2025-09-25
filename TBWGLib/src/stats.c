#include <TBWG/stats.h>


struct Stats tbwgSumStats(struct Stats a, struct Stats b)
{
    int* a_ = (int*)&a;
    int* b_ = (int*)&b;

    for (unsigned int i = 0 ; i < sizeof(a)/sizeof(int); i += 1) {
        a_[i] += b_[i];
    }

    return a;
}