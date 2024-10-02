#define ENTITY_DEFAULT 0x01
#define ENTITY_ROCK 0x02

struct Entity {
	digits32 entityCode;
	int x,y;
	float dirx, diry;
};