#ifndef TBWG_EFFECTS_H
#define TBWG_EFFECTS_H

#include <TBWG/essentials.h>
#include <TBWG/stats.h>
#include <TBWG/lists.h>

struct World;
struct Character;

#define EFFECT_STATGIVER (1<<0)
#define EFFECT_INVISIBLE (1<<1)

#define INVOKE_ON_REORDER (1<<0)
#define INVOKE_ON_CHARACTERTURN (1<<1)

#define EFFECTOR_CHARACTER 0x1
#define EFFECTOR_DIMENSION 0x2
#define EFFECTOR_AREA 0x03

#define EFFECT_TRIGGER_TYPE_COUNT 5
#define EFFECT_TRIGGER_TYPE_CLOCK 0x00
#define EFFECT_TRIGGER_TYPE_TURN 0x01
#define EFFECT_TRIGGER_TYPE_HIT 0x02
#define EFFECT_TRIGGER_TYPE_BEFORE_STATGIVING 0x03
#define EFFECT_TRIGGER_TYPE_AFTER_STATGIVING 0x04

void defaultEffectExecuter(void* effectptr, struct World*, struct Character* entity, void* relativeInformation);

struct Effect {
	id_number ID;
	uint32_t code;
	uint8_t effectorType;
	id_number effectorId; // player-
	void* effector;

	int time;
	int willberemoved; // if thats 1, executer loop will remove this effect

	digits32 effectSpecs;

	struct Stats givenStats;

	void (*executer)(void* effectptr, struct World*, struct Character* entity, void* relativeInformation);

	int details[8];
};


struct EffectListElement {
	struct ListElementHeader header;
	struct Effect* effect;
};

struct Effect* getTestingHitEffect(void* effector); //

#endif /*TBWG_EFFECTS_H*/