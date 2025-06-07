#ifndef TBWG_MATHS_H
#define TBWG_MATHS_H

struct iVector_ {
	int x,y;
};

struct fVector_ {
	float x,y;
};

struct iArea_ {
	struct iVector_ a, b;
};

typedef struct iVector_ iVector;
typedef struct fVector_ fVector;
typedef struct iArea_ iArea;

iVector getiVector(int x, int y);
fVector getfVector(float x, float y);

int isInVisionArea(fVector direction, float seeingAngle, iVector position, iVector opp_position); // radians

float getiVectorLength(iVector);
float getfVectorLength(fVector);

fVector normiVector(iVector);

iVector connectiVectors(iVector from, iVector to);
fVector connectfVectors(fVector from, fVector to);

iVector addiVectors(iVector a, iVector b);

float getiVectorDistance(iVector a, iVector b);

float getMaxLookLengthByAngle(float seeingAngle); // radians

float getVisionHardnessFinal(float visionHardness, float distance);


int isPositionInside(iVector pos, iVector a, iVector b);

#endif /*TBWG_MATHS_H*/