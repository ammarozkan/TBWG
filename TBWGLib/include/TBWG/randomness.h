#ifndef TBWG_RANDOMNESS
#define TBWG_RANDOMNESS

void tbwgRandomnessInit();
int tbwgGetRandomed(unsigned int* masses, unsigned int count); // returns which one is the one

int tbwgGetRandomed_2(unsigned int a, unsigned int b);

int tbwgDice(int d);

int tbwgRandomNumber(int min, int max);

int tbwgRandomPercentageIncrease(int value, int min, int max);

#endif