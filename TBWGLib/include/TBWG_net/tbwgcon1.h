#include <stdint.h>

#include <TBWG_net/tbwgcon1_essentials.h>

void tbwgcon1InitGlobalRecvPtr();

int tbwgcon1SetHeader(struct TBWGConHeader);

int tbwgcon1GetProperServerSocket(char* ip_c, uint16_t port);
int tbwgcon1GetProperClientSocket(char* ip_c, uint16_t port);

int tbwgcon1HeaderCheck(struct TBWGConHeader);

int tbwgcon1ReceivePackage(int socket, void* memptr, uint8_t pkgcode);
int tbwgcon1SendPackage(int socket, void* memptr, uint8_t pkgcode);

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);




int tbwgcon1Accept(int sv_sock);
int tbwgcon1Connect(char* ip_c, uint16_t port);
int tbwgcon1Close(int sock);