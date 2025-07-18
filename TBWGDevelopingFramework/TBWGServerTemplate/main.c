#include <stdio.h>
#include <stdlib.h> // strtoul, rand

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/entity.h>
#include <TBWG/maths.h>
#include <TBWG/randomness.h>

#include <TBWG_net/tbwgcon1.h>

#define MAX_PLAYER_COUNT 16

char* ip_addr = "127.0.0.1";
uint16_t port = 5005;
unsigned int rounds = 50;

#include "details/tgfEngine.h"

#include "details/tgfSystemArguments.h"

#include "morsels/morsels.h"

void worldGenerator()
{
	morselsWorldGeneration(4);
}

int main(int argc, char*argv[])
{
	tbwgRandomnessInit();
	tgfSystemArguments(argc, argv);
	tgfEngine(MAX_PLAYER_COUNT, worldGenerator);
	
	return 0;
}
