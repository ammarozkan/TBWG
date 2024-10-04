#ifndef TBWG_OBSERVATION_H
#define TBWG_OBSERVATION_H

#include <TBWG/essentials.h>
#include <TBWG/world.h>

struct CharacterInformation {
	id_number ID;
	unsigned int characterCode;
	int x,y;
	int hp;
	float dirx,diry;
};

struct EntityInformation{
    id_number ID;
    unsigned int entityCode;
    int x,y;
    float dirx,diry;
};

struct ObservingInformation{
    id_number selfid;

    struct Stats characterStats;
    int hp, maxhp;
    int e, maxe;
    int se, maxse;
    unsigned int state;

    struct List effects;

    size_t eventerCount;
    struct Eventer* eventers;


    size_t characterCount;
    struct CharacterInformation* charInfos;

    size_t entityCount;
    struct EntityInformation* entityInfos;
};

struct ObservingInformation observe(struct Character* as, struct World* world);

#endif /*TBWG_OBSERVATION_H*/