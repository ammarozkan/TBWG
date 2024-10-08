#ifndef TBWG_OBSERVATION_H
#define TBWG_OBSERVATION_H

#include <TBWG/essentials.h>
#include <TBWG/world.h>
#include <TBWG/stats.h>

struct CharacterInformation {
	id_number ID;
	unsigned int characterCode;
	int x,y;
	int hp;
	float dirx,diry;
};

struct EntityInformation {
    id_number ID;
    unsigned int entityCode;
    int x,y;
    float dirx,diry;
};

struct ObservingInformation {
    id_number selfid;

    struct Stats characterStats;
    iValue hp, e, se;
    digits32 state;

    struct List effects;

    size_t eventerCount;
    struct Eventer* eventers;


    size_t characterCount;
    struct CharacterInformation* charInfos;

    size_t entityCount;
    struct EntityInformation* entityInfos;
};

typedef int (*SeeCharacter)(struct Character* observer, struct Character* target);
typedef int (*CanSeen)(struct Character* observer, struct Character* target);

struct ObservingInformation Observe(struct Character* as, struct World* world);


#endif /*TBWG_OBSERVATION_H*/