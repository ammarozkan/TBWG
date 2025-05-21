#include <stdio.h>
#include <stdlib.h>


#include <TBWG_net/tbwgcon1.h>


uint8_t CharacterSelector(struct TBWGConCharacterInformator cinfer)
{
selectacharacter:
	printf("SELECT A CHARACTER:\n");
	for(unsigned int i = 0 ; i < cinfer.characterCount ; i += 1) {
		struct TBWGConCharacterInformation inf = cinfer.charinfo[i];
		printf("%u:Character[%u]\n",i,inf.code);
	}
	uint8_t selection = 0;
	scanf("%u",&selection);
	if (selection < cinfer.characterCount) return selection;
	else {
		printf("Beyond limit.\n");
		goto selectacharacter;
	}
}

int main()
{
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	struct TBWGConClientResult res;
	tbwgcon1SetHeader(header);
	
	printf("globalrecvptr\n");
	tbwgcon1InitGlobalRecvPtr();

	printf("Enter a name:");
	char name[64];
	scanf("%s",name);
	res = tbwgcon1Connect("127.0.0.1", 5045, name, CharacterSelector);

	if (res.socket < 0) {
		printf("Failed connection. (errcode:%i)\n",res.socket);
		return -1;
	}

	printf("GREAT!\n");

	printf("NAME IS %s and CHARACTER IS %u!\n", name, res.inf.code);

	struct TBWGConHeader* GLB_RECV = calloc(1024,0);
	while(1) {
		int r = tbwgcon1ReceivePackage(res.socket, GLB_RECV, 0);
		if (r == -2) break;
		printf("NEW PACKAGE! %i (%s:%u:%u)\n",r,GLB_RECV->tbwgname,GLB_RECV->version[0],GLB_RECV->pkgcode);
		if(GLB_RECV->pkgcode == TBWGCON1_EVENTEROPTIONSINFORMATIONHEADER) {
			r = tbwgcon1ReceivePackage(res.socket, GLB_RECV, 0); // reading the options
			if(GLB_RECV->pkgcode != TBWGCON1_EVENTEROPTIONSINFORMATION) printf("SOMETHINGS NOT GOING TOO GOOD TOO LIVING TOO BEING TOO GREAT!\n");

			struct EventerRequiredInformations reqs = {getiVector(0,0), getiVector(0,0), getfVector(1.0f, 0.0f), getiVector(0,0), getiVector(1,1)};
			struct TBWGConTurnPlay ctp = {.eventer_th = 0, .requiredInformations = reqs, .specs = TURNPLAY_END_TURN};
			tbwgcon1SendPackage(res.socket, &ctp, TBWGCON1_TURNPLAY, sizeof(ctp));
			printf("OK with OPTINER!\n");
		}
	}
	return 0;
}