#include <string.h>
#include "tools.h"



typedef void (*tgfWorldGeneration)();

void tgfInit()
{
	// TBWG

	printf("Initializing TBWG\n");
	tbwgInit();
	tbwgReorder();

	// TBWG NET
	tbwgcon1Init();
	printf("TBWGCON header initializing.\n");
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	tbwgcon1SetHeader(header);

	printf("globalrecvptr\n");
	tbwgcon1InitGlobalRecvPtr();
}



void tgfPreGame(int server_socket, unsigned int maxPlayerCount)
{
	unsigned int cmd = 0;
	
	// Pre-Game

	for (unsigned int i = 0 ; i < maxPlayerCount ; i += 1) {
		printf("Creating character infos list.\n");
		struct List charinfoList = getAvailableCharacterInfos(tbwgGetFirstDimension());
		printf("Awaiting for connection\n");
		struct TBWGConServerResult res = tbwgcon1Accept(server_socket, charinfoList, NULL);
		printf("Decolonizing charinfoList\n");
		decolonizeList(&charinfoList);

		if(res.socket < 0) {
			printf("Failed connection. (err:%i)\n",res.socket);
			return;
			i-=1;
			continue;
		}

		printf("GREAT CONNECTION! Hello %s.\n",res.name);
		// network interface initialization
		struct ControllerInterface* intf = tbwgcon1GetNetworkedControllerInterface(res.socket,res.name);
		struct Character* chr = (struct Character*)(res.midinf.systematicPtr);
		chChangeControllerInterface(chr, intf);

waitingforcommandonserver:
		printf("0:Wait for New Player\n");
		printf("1:Start the Game\n");
		printf("cmd:"); scanf("%u",&cmd);
		if (cmd == 0) continue;
		else if (cmd == 1) break;
		else goto waitingforcommandonserver;
	}

}

void tgfTbwgGame()
{
	// GAME
	printf("GAME!\n");

	for(unsigned int i = 0 ; i < rounds ; i += 1) {
		printf("%uth turn\n",i);
		tbwgTurn();
	}
	printf("Server is closing.\n");
}

void tgfEngine(unsigned int maxPlayerCount, tgfWorldGeneration wg)
{
	tgfInit();

	wg();
	printf("Using host %s:%u\n",ip_addr,port);
	printf("Max players:%u\n",maxPlayerCount);

	printf("Server is starting.\n");
	TbwgConSocket server_socket = tbwgcon1GetProperServerSocket(ip_addr, port);
	if (!ISSOCKETOK(server_socket)) {
		printf("SOCKET NOT OK! %i\n", server_socket);
		return;
	}
	
	tgfPreGame(server_socket, maxPlayerCount);
	tgfTbwgGame();

	tbwgcon1Close(server_socket);

}