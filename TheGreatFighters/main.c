#include <stdio.h>
#include <stdlib.h> // strtoul, rand

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/maths.h>

#include <TBWG_net/tbwgcon1.h>

#define MAX_PLAYER_COUNT 16

char* ip_addr = "127.0.0.1";
uint16_t port = 5005;
unsigned int rounds = 50;

#include "basicmovements.h"

#include "details/tgfEngine.h"

#include "details/tgfSystemArguments.h"

#define TGF_MORSELS

#ifdef TGF_MORSELS
#include "morsels/morsels.h" // dont include this if you dont want any mods
#endif

struct Character* createRandomCharacter()
{
	static unsigned int i = 0;
	iVector pos = getiVector(0,0);
	if (i == 0) pos = getiVector(1,0);
	else if (i == 1) pos = getiVector(0,1);
	else if (i == 2) pos = getiVector(-1,0);
	else if (i == 3) pos = getiVector(0,-1);
	else if (i == 4) pos = getiVector(1,1);
	else if (i == 5) pos = getiVector(-1,-1);
	i+=1;
	return createDefaultHuman(tbwgGetFirstDimension(), pos);
}

void worldGenerator()
{
	for (unsigned int i = 0 ; i < 2 ; i += 1) tbwgAddCharacter(createRandomCharacter());

	#ifdef TGF_MORSELS
	tgfMorselsWorldGeneration(2);
	#endif
}

int main(int argc, char*argv[])
{
	tgfSystemArguments(argc, argv);
	tgfEngine(MAX_PLAYER_COUNT, worldGenerator);

	return 0;
}
