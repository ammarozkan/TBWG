#ifdef TBWG_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(func,txt) printf("DEBUG:%s %s\n",func,txt)
#define DEBUG_PRINTINT(func,txt,int) printf("DEBUG:%s %s ([%i])\n",func,txt,int)
#define DEBUG_PRINTUINT(func,txt,uint) printf("DEBUG:%s %s ([%u])\n",func,txt,uint)
#define DEBUG_RECEIVEDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode) printf("DEBUG:%s (%u)%u sized (%u)%u code pkg reaceived\n", func, targetsize, recvsize, targetpkgcode, pkgcode)
#define DEBUG_SENDDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode) printf("DEBUG:%s (%u)%u sized (%u)%u code pkg sent\n", func, targetsize, recvsize, targetpkgcode, pkgcode)
#else
#define DEBUG_PRINT(x,y)
#define DEBUG_PRINTINT(func,txt,int)
#define DEBUG_PRINTUINT(func,txt,uint)
#define DEBUG_RECEIVEDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode)
#define DEBUG_SENDDEBUG(func,targetsize,recvsize,targetpkgcode,pkgcode)
#endif /*TBWG_DEBUG*/


#define TBWGCON1_GLOBALRECVPTR_SIZE 4096
extern void* globalrecvptr;
#define GLB_RECV globalrecvptr
#define GLB_RECV_HEAD ((struct TBWGConHeader*)GLB_RECV)


#include <stdint.h>

#include <TBWG_net/tbwgcon1_essentials.h>

void tbwgcon1InitGlobalRecvPtr();

int tbwgcon1SetHeader(struct TBWGConHeader);
int tbwgcon1SetDefaultHeader();

struct TBWGConHeader tbwgcon1GetHeader(uint8_t pkgcode);

int tbwgcon1GetProperServerSocket(char* ip_c, uint16_t port);
int tbwgcon1GetProperClientSocket(char* ip_c, uint16_t port);

int tbwgcon1HeaderCheck(struct TBWGConHeader);

int tbwgcon1ReceivePackage(int socket, void* memptr, uint8_t pkgcode);
int tbwgcon1SendPackage(int socket, void* memptr, uint8_t pkgcode, uint32_t size);

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);

typedef uint8_t (*tbwgcon1CharacterSelector)(struct TBWGConCharacterInformator);

struct TBWGConServerResult tbwgcon1Accept(int sv_sock, struct List characterList, void* decidersptr);
struct TBWGConClientResult tbwgcon1Connect(char* ip_c, uint16_t port, char* name, tbwgcon1CharacterSelector);
int tbwgcon1Close(int sock);


// CONTROLLER INTERFACE
#include <TBWG/controllerInterface.h>

struct tbwgcon1ControllerInterface {
	struct ControllerInterface interface;
	int cl_sck;
	char* name;
};

void tbwgcon1ControllerObserve(struct ControllerInterface*, struct ObservingInformation);
void tbwgcon1ReceiveWorldEvent(struct ControllerInterface*, struct WorldEventInformation);
struct TurnPlay tbwgcon1ControllerChooseEventer(struct ControllerInterface*, id_number chooserId, 
    digits32 allowedEventerTypes, size_t eventerCount, struct Eventer** eventers, struct EventerUses restUses);


struct ControllerInterface* tbwgcon1GetNetworkedControllerInterface(int socket, char* name);