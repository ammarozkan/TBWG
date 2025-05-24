#include <stdio.h>
#include <stdlib.h>


#include <TBWG_net/tbwgcon1.h>


struct TBWGConHeader* CL_GLB_RECV;

uint8_t CharacterSelector(struct TBWGConCharacterInformator cinfer);
int cstdTBWGPackageManager(struct TBWGConClientResult res);

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

	CL_GLB_RECV = calloc(1024,0);
	while(1) {
		if(cstdTBWGPackageManager(res) < 0) break;
	}
	return 0;
}


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


int cstdTBWGObservingInformation(struct TBWGConClientResult res, struct TBWGConObservingInformationHeader hdr)
{
	int r = tbwgcon1ReceivePackage(res.socket, GLB_RECV, 0); // reading the obsinf
	struct TBWGConObservingInformation* inf = (struct TBWGConObservingInformation*)GLB_RECV;
	
	printf("\nObservingInformation for %u\n",inf->selfid);
	printf("HP:%u/%u E:%u/%u SE:%u/%u\n", inf->hp.value, inf->hp.max, inf->e.value, inf->e.max, inf->se.value, inf->se.max);
	printf("Pos:(%i,%i) Rot:(%f,%f) St:%u\n", inf->position.x, inf->position.y, inf->direction.x, inf->direction.y, inf->state);

	struct TBWGConEffectInformation* effects[EFFECT_TRIGGER_TYPE_COUNT];
	effects[0] = (GLB_RECV+sizeof(struct TBWGConObservingInformation));
	for (unsigned int i = 1 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		effects[i] = effects[i-1] + hdr.effectCounts[i-1];
	}
	void* eventersstart = (void*)(effects[EFFECT_TRIGGER_TYPE_COUNT-1] + hdr.effectCounts[EFFECT_TRIGGER_TYPE_COUNT-1]);
	struct TBWGConUsersEventerInformation* eventers = (struct TBWGConUsersEventerInformation*)eventersstart;
	struct CharacterInformation* characters = (struct CharacterInformation*)(eventers + hdr.eventerCount);
	struct EntityInformation* entities = (struct EntityInformation*)(characters + hdr.characterInformationCount);

	for(unsigned int i = 0 ; i < hdr.characterInformationCount ; i += 1) {
		struct CharacterInformation chr = characters[i];
		printf("\tCharacter %u with ID %u on (%i,%i)\n", chr.characterCode, chr.ID, chr.position.x, chr.position.y);
	}
	printf("%u%u%u%u%u%u%u\n\n",inf->selfid,inf->selfid,inf->selfid,inf->selfid,inf->selfid,inf->selfid,inf->selfid);

	return 1;
}

int cstdTBWGWorldEventInformation(struct TBWGConWorldEventInformation wei)
{
	printf("EVENT %s on (%i,%i)\n", wei.eventName, wei.position.x, wei.position.y);
	return 1;
}

struct EventerRequiredInformations selectRequiredEventerInformations(struct TBWGConEventerInformation e)
{
	struct EventerRequiredInformations r;
	if (e.required_informations & EVENTER_REQUIRED_INFORMATION_POSITION) {
		printf("x:");scanf("%u",&(r.position.x));
		printf("y:");scanf("%u",&(r.position.y));	
	} /*and*/ if (e.required_informations & EVENTER_REQUIRED_INFORMATION_DIRECTION) {
		printf("dirx:");scanf("%f",&(r.direction.x));
		printf("diry:");scanf("%f",&(r.direction.y));	
	}
	return r;
}

int cstdTBWGEventerOptions(struct TBWGConClientResult res, struct TBWGConEventerOptionsInformationHeader hdr)
{
	int r = tbwgcon1ReceivePackage(res.socket, CL_GLB_RECV, 0); // reading the options
	if(CL_GLB_RECV->pkgcode != TBWGCON1_EVENTEROPTIONSINFORMATION) 
		printf("SOMETHINGS NOT GOING TOO GOOD TOO LIVING TOO BEING TOO GREAT!\n");
	struct TBWGConEventerOptionsInformation* optinf = (struct TBWGConEventerOptionsInformation*)CL_GLB_RECV;
	struct TBWGConEventerInformation* eventers = (struct TBWGConEventerInformation*)(optinf+1);
	for (unsigned int i = 0 ; i < hdr.eventerCount ; i += 1) {
		struct TBWGConEventerInformation eventer = eventers[i];
		printf("\t%uth EVENTER %s\n", i, eventer.name);
	}
	
	unsigned int eventer_th = 0;
	
	printf("eventer_th:"); scanf("%u",&eventer_th);

	uint32_t specs = 0;
	if (eventer_th >= hdr.eventerCount) specs = specs | TURNPLAY_END_TURN;

	struct EventerRequiredInformations reqs = selectRequiredEventerInformations(eventers[eventer_th]);

	struct TBWGConTurnPlay ctp = {.eventer_th = eventer_th, .requiredInformations = reqs, .specs = specs};
	tbwgcon1SendPackage(res.socket, &ctp, TBWGCON1_TURNPLAY, sizeof(ctp));
}

int cstdTBWGPackageManager(struct TBWGConClientResult res)
{
	int r = tbwgcon1ReceivePackage(res.socket, CL_GLB_RECV, 0);
	if (r == -2) return -1;

	//printf("NEW PACKAGE! %i (%s:%u:%u)\n",r,CL_GLB_RECV->tbwgname,CL_GLB_RECV->version[0],CL_GLB_RECV->pkgcode);

	if(CL_GLB_RECV->pkgcode == TBWGCON1_EVENTEROPTIONSINFORMATIONHEADER) {
		cstdTBWGEventerOptions(res, *(struct TBWGConEventerOptionsInformationHeader*)(CL_GLB_RECV) );
	} else if (CL_GLB_RECV->pkgcode == TBWGCON1_OBSERVINGINFORMATIONHEADER) {
		return cstdTBWGObservingInformation(res,*(struct TBWGConObservingInformationHeader*)(CL_GLB_RECV));
	} else if (CL_GLB_RECV->pkgcode == TBWGCON1_WORLDEVENTINFORMATION) {
		struct TBWGConWorldEventInformation* wei = (struct TBWGConWorldEventInformation*)CL_GLB_RECV;
		return cstdTBWGWorldEventInformation(*wei);
	}

	return 1;
}