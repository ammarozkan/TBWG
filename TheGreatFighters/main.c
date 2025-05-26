#include <stdio.h>
#include <stdlib.h>

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/maths.h>

#include <TBWG_net/tbwgcon1.h>

#include "tools.h"

#include "basicmovements.h"

void init();
struct Character* createRandomCharacter();
void ENTERTOCONT();

#define MAX_PLAYER_COUNT 16
int main(int argc, char*argv[])
{
	init();

	printf("Server is starting.\n");
	char* ip_addr = malloc(64);
	uint16_t port;
	printf("ip_addr:");scanf("%s",ip_addr);
	printf("port:");scanf("%u",&port);
	int server_socket = tbwgcon1GetProperServerSocket(ip_addr, port);
	free(ip_addr);

	// Pre-Game

	unsigned int characterCount = 0;
	printf("CharacterCount:"); scanf("%u",&characterCount);
	for (unsigned int i = 0 ; i < characterCount ; i += 1) tbwgAddCharacter(createRandomCharacter());

	for (unsigned int i = 0 ; i < MAX_PLAYER_COUNT ; i += 1) {
		printf("Creating character infos list.\n");
		struct List charinfoList = getAvailableCharacterInfos(tbwgGetFirstDimension());
		printf("Awaiting for connection\n");
		struct TBWGConServerResult res = tbwgcon1Accept(server_socket, charinfoList, NULL);
		printf("Decolonizing charinfoList\n");
		decolonizeList(&charinfoList);

		if(res.socket < 0) {
			printf("Failed connection. (err:%i)\n",res.socket);
			i-=1;
			continue;
		}

		printf("GREAT CONNECTION! Hello %s.\n",res.name);
		// network interface initialization
		struct ControllerInterface* intf = tbwgcon1GetNetworkedControllerInterface(res.socket,res.name);
		struct Character* chr = (struct Character*)(res.midinf.systematicPtr);
		chChangeControllerInterface(chr, intf);

waitingforcommandonserver:
		unsigned int cmd = 0;
		printf("0:Wait for New Player\n");
		printf("1:Start the Game\n");
		printf("cmd:"); scanf("%u",&cmd);
		if (cmd == 0) continue;
		else if (cmd == 1) break;
		else goto waitingforcommandonserver;
	}

	// GAME
	printf("GAME!\n");

	for(unsigned int i = 0 ; i < 16 ; i += 1) {
		printf("%uth turn\n",i);
		tbwgTurn();
		//ENTERTOCONT();
	}
	printf("Server is closing.\n");
	tbwgcon1Close(server_socket);
	return 0;
}

void init()
{
	// TBWG

	printf("Initializing TBWG\n");
	tbwgInit();
	tbwgReorder();

	// TBWG NET
	printf("TBWGCON header initializing.\n");
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	tbwgcon1SetHeader(header);

	printf("globalrecvptr\n");
	tbwgcon1InitGlobalRecvPtr();
}

void ENTERTOCONT()
{
    printf("Please press ENTER to continue\n");

    char myChar = 0;
    while (myChar != '\n' && myChar  != '\r') { 
        myChar = getchar(); 
    }
}

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
	return createDefaultTheGreatCharacter(tbwgGetFirstDimension(), pos);
}