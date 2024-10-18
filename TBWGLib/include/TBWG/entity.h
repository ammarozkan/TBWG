#ifndef TBWG_ENTITY_H
#define TBWG_ENTITY_H

#include<TBWG/characters.h>
#include <TBWG/being.h>

#define ENTITY_DEFAULT 0x01
#define ENTITY_ROCK 0x02

struct Entity;

typedef void (*UseEntityFunction)(struct Entity*, struct Being* user);

struct Entity {
	struct Being b;

	HitterFunction hit;
	UseEntityFunction use;
};


struct Entity* createDefaultEntity(struct Dimension* dimension, iVector position);
void destroyEntity(struct Entity*);


#endif /*TBWG_ENTITY_H*/