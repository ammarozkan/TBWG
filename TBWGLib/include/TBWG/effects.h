#include <TBWG/essentials.h>

#define EFFECT_STATGIVER (1<<0)
#define EFFECT_INVISIBLE (1<<1)

#define INVOKE_ON_REORDER (1<<0)
#define INVOKE_ON_CHARACTERTURN (1<<1)

#define EFFECTOR_PLAYER 0x1
#define EFFECTOR_DIMENSION 0x2

struct Effect {
	unsigned int ID;
	uint8_t effectorType;
	id_number effectorId; // player-

	int time;

	digits32 effectSpecs;

	struct Stats givenStats;

	digits32 invokeTimeType;

	struct Character* character;
	void (*executer)(void* effectptr, struct World*, struct Character* entity);
	
};