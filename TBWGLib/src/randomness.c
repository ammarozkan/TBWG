#include <TBWG/randomness.h>

#include <time.h> // time
#include <stdlib.h> // rand, srand


void tbwgRandomnessInit()
{
    srand(time(NULL));
}


int tbwgGetRandomed(unsigned int* masses, unsigned int count)
{
    int max = 0;
    for(unsigned int i = 0 ; i < count ; i += 1) {
        max += masses[i];
    }
    int r = rand()%max;

    int c = 0;
    for(unsigned int i = 0 ; i < count ; i += 1) {
        c += masses[i];
        if (r < c) {
            return i;
        }
    }

    return (int)count;
}

int tbwgGetRandomed_2(unsigned int a, unsigned int b)
{
    unsigned int masses[2] = {a,b};
    return tbwgGetRandomed(masses, 2);
}

int tbwgDice(int d)
{
    return rand()%d + 1;
}

int tbwgRandomNumber(int min, int max)
{
    return (rand()%(max-min)) + min;
}

int tbwgRandomPercentageIncrease(int value, int min, int max)
{
    float v = (float)value;
    float prc = (float)tbwgRandomNumber(min, max);
    float result = v+v*(prc/100.0f);
    return (int)result;
}