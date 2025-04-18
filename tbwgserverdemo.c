#include <stdio.h>
#include <stdlib.h>

#include <TBWG/essentials.h>
#include <TBWG/lists.h>
#include <TBWG/tbwgmanager.h>
#include <TBWG/characters.h>
#include <TBWG/maths.h>

#include <TBWG_net/tbwgcon1.h>

struct List getAvailableCharacterInfos()
{
	struct Dimension* dimension = tbwgGetFirstDimension();

	struct List ptsizedCharInfos = createList();

	ITERATE(dimension->characterList, charListElm_pure) {
		struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;

		if (!tbwgIsDefaultControllerInterface(charListElm->character->controllerInterface)) continue;


		struct TBWGConCharacterInformation inf = {.code = charListElm->character->b.code};
		struct TBWGConPtsizedCharacterInformationListElement elm = {.charinf.systematicPtr = charListElm->character, .charinf.inf = inf};
		addElement(&ptsizedCharInfos, &elm, sizeof(elm));
		printf("Empty character found on first dimension! Code:%u\n",elm.charinf.inf.code);
	}
	return ptsizedCharInfos;
}

#define PLAYER_COUNT 1
int main(int argc, char*argv[])
{
	// TBWG

	printf("Initializing TBWG\n");
	tbwgInit();
	tbwgReorder();

	printf("Adding characters.\n");
	struct Character* character1 = createDefaultCharacter(tbwgGetFirstDimension(), getiVector(1,0));
	tbwgAddCharacter(character1);

	// TBWG NET
	printf("TBWGCON header initializing.\n");
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	tbwgcon1SetHeader(header);

	printf("globalrecvptr\n");
	tbwgcon1InitGlobalRecvPtr();

	printf("Server is starting.\n");
	int server_socket = tbwgcon1GetProperServerSocket("127.0.0.1", 5045);

	for (unsigned int i = 0 ; i < PLAYER_COUNT ; i += 1) {
		printf("Creating character infos list.\n");
		struct List charinfoList = getAvailableCharacterInfos();
		printf("Awaiting for connection\n");
		struct TBWGConServerResult res = tbwgcon1Accept(server_socket, charinfoList, NULL, NULL);
		printf("Decolonizing charinfoList\n");
		decolonizeList(&charinfoList);

		if(res.socket < 0) {
			printf("Failed connection. (err:%i)\n",res.socket);
			i-=1;
			continue;
		}

		printf("GREAT CONNECTION! Hello %s.\n",res.name);
		// network interface initialization here
	}

	// GAME
	printf("GAME!\n");

	for(unsigned int i = 0 ; i < 16 ; i += 1) {
		printf("%uth turn\n",i);
		tbwgTurn();
	}
	printf("Server is closing.\n");
	tbwgcon1Close(server_socket);
	return 0;
}
