#define TBWGCON1_GLOBALRECVPTR_SIZE 4096
extern void* globalrecvptr;
#define GLB_RECV globalrecvptr
#define GLB_RECV_HEAD ((struct TBWGConHeader*)GLB_RECV)


#include <stdint.h>

#include <TBWG_net/tbwgcon1_essentials.h>

#ifdef _WIN32
#include <winsock2.h>
typedef SOCKET TbwgConSocket;
#define ISSOCKETOK(socket) (socket != INVALID_SOCKET)
#define TBWGCON1_BROKEUPSERVERWITHERROR(str,errcode) {closesocket(cl_fd);DEBUG_PRINT("tbwgcon1Accept",str);return tbwgcon1GetSvError(errcode);}
#define TBWGCON1_BROKEUPCLIENTWITHERROR(str,errcode) {closesocket(cl_fd); DEBUG_PRINT("tbwgcon1Connect", str); return tbwgcon1GetClError(errcode); }
#else
typedef int TbwgConSocket;
#define ISSOCKETOK(socket) (socket > 0)
#define TBWGCON1_BROKEUPSERVERWITHERROR(str,errcode) {close(cl_fd);DEBUG_PRINT("tbwgcon1Accept",str);return tbwgcon1GetSvError(errcode);}
#define TBWGCON1_BROKEUPCLIENTWITHERROR(str,errcode) {close(cl_fd); DEBUG_PRINT("tbwgcon1Connect", str); return tbwgcon1GetClError(errcode); }
#endif

int tbwgcon1Init(); // important for winsock
void tbwgcon1Dinit(); // important for winsock

void tbwgcon1InitGlobalRecvPtr();

int tbwgcon1SetHeader(struct TBWGConHeader);
int tbwgcon1SetDefaultHeader();

struct TBWGConHeader tbwgcon1GetHeader(uint8_t pkgcode);

TbwgConSocket tbwgcon1GetProperServerSocket(char* ip_c, uint16_t port);
TbwgConSocket tbwgcon1GetProperClientSocket(char* ip_c, uint16_t port);

int tbwgcon1HeaderCheck(struct TBWGConHeader);

int tbwgcon1ReceivePackage(int socket, void* memptr, uint8_t pkgcode);
int tbwgcon1SendPackage(int socket, void* memptr, uint8_t pkgcode, uint32_t size);

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);

typedef uint8_t (*tbwgcon1CharacterSelector)(struct TBWGConCharacterInformator);

struct TBWGConServerResult tbwgcon1Accept(int sv_sock, struct List characterList, void* decidersptr);
struct TBWGConClientResult tbwgcon1Connect(char* ip_c, uint16_t port, char* name, tbwgcon1CharacterSelector);
int tbwgcon1Close(TbwgConSocket sock);


// CONTROLLER INTERFACE
#include <TBWG/controllerInterface.h>

struct tbwgcon1ControllerInterface {
	struct ControllerInterface controllerInterface;
	int cl_sck;
	char* name;
};

void tbwgcon1ControllerObserve(struct ControllerInterface*, struct ObservingInformation);
void tbwgcon1ReceiveWorldEvent(struct ControllerInterface*, struct WorldEventInformation);
struct TurnPlay tbwgcon1ControllerChooseEventer(struct ControllerInterface*, id_number chooserId, 
    digits32 allowedEventerTypes, size_t eventerCount, struct Eventer** eventers, struct EventerUses restUses);


struct ControllerInterface* tbwgcon1GetNetworkedControllerInterface(int socket, char* name);