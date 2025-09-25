#ifndef TBWG_OBSERVATION_H
#define TBWG_OBSERVATION_H

#include <TBWG/essentials.h>
#include <TBWG/world.h>
#include <TBWG/stats.h>
#include <TBWG/maths.h>
#include <TBWG/worldevents.h>
#include <TBWG/effects.h>


struct CharacterInformation {
	id_number ID;
	unsigned int characterCode;
	iVector position;
    fVector direction;
	iValue hp;
};

struct EntityInformation {
    id_number ID;
    unsigned int entityCode;
    iVector position;
    fVector direction;
};

struct ToolInformation {
    unsigned int toolCode;
    Details details;
    uint8_t observationPower; // if 0, its nothing; if 1, just the tool code, if 2, the details too baby
};


struct SeeingResourceElement {
    struct ListElementHeader header;
    void* resource;
};

struct Eye {
    float angle;
    float level;
    float speed;
    float hearingLevel;
    float hearingSpeed;
};

struct ObservingInformation {
    id_number selfid;

    struct Stats characterStats;
    iValue hp, e, se;
    iVector position; fVector direction;
    digits32 state;
    struct ToolInformation tool;

    struct List effects[EFFECT_TRIGGER_TYPE_COUNT];

    size_t eventerCount;
    struct Eventer* *eventers;


    size_t characterCount;
    struct CharacterInformation* charInfos;

    size_t entityCount;
    struct EntityInformation* entityInfos;
};

struct ObservingInformation Observe(struct Character* as, struct World* world);

struct WorldEventInformation {
    id_number selfid;
    char* eventName;
    iVector position;
};

struct WorldEventInformation ObserveWorldEventInformation(struct Character* as, struct WorldEvent*);

int canSeeCharacter(struct Character*, struct Character* target);

#endif /*TBWG_OBSERVATION_H*/