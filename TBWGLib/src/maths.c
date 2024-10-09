#include <TBWG/maths.h>
#include <math.h>


iVector getiVector(int x, int y)
{
	iVector result = {x,y};
	return result;
}

fVector getfVector(float x, float y)
{
	fVector result = {x,y};
	return result;
}

float getiVectorDistance(iVector a, iVector b)
{
	return getiVectorLength(connectiVectors(a, b));
}

float getiVectorLength(iVector v)
{
	float x = (float)v.x;
	float y = (float)v.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

float getfVectorLength(fVector v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

fVector normiVector(iVector v)
{
	fVector v_ = {(float)v.x, (float)v.y};

	float length = getfVectorLength(v_);
	fVector result = {v_.x/length, v_.y/length};
	return result;
}

iVector connectiVectors(iVector from, iVector to)
{
	iVector result = {to.x - from.x, to.y - from.y};
	return result;
}

fVector connectfVectors(fVector from, fVector to)
{
	fVector result = {to.x - from.x, to.y - from.y};
	return result;
}

int isInVisionArea(fVector direction, float seeingAngle, iVector position, iVector opp_position)
{
	fVector target_direction = normiVector(connectiVectors(position, opp_position));
	fVector lookVector = connectfVectors(direction, target_direction);
	float lookLength = getfVectorLength(lookVector);
	float angleMaxLookLength = getMaxLookLengthByAngle(seeingAngle);
	return lookLength < angleMaxLookLength;
}

float getMaxLookLengthByAngle(float seeingAngle)
{
	fVector maxLookVector = {cos(seeingAngle/2.0f) - 1.0f, sin(seeingAngle/2.0f)};
	return getfVectorLength(maxLookVector);
}


float getVisionHardnessFinal(float visionHardness, float distance)
{
	float distanceFactor = fmax(1.0f, pow(distance, 0.25f)/2.0f);
	return visionHardness * distanceFactor;
}