#include <TBWG_net/tbwgcon1_essentials.h>


int tbwgcon1SetHeader(struct TBWGConHeader);

int tbwgcon1GetProperServerSocket(uint32_t ip, uint16_t port);
int tbwgcon1GetProperClientSocket(uint32_t ip, uint16_t port);

int tbwgcon1ReceivePackage(int socket, void* memptr, uint8_t pkgcode);
int tbwgcon1SendPackage(int socket, void* memptr, uint8_t pkgcode);

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);