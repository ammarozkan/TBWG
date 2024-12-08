#include <TBWG_net/tbwgcon1.h>

int pkgcodesize[64] = {};

struct TBWGConHeader header;

int tbwgcon1SetHeader(struct TBWGConHeader h)
{
	header = h;
	return 1;
}


int tbwgcon1GetProperServerSocket(uint32_t ip, uint16_t port);
int tbwgcon1GetProperClientSocket(uint32_t ip, uint16_t port);


int tbwgcon1ReceivePackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	// this function should see the pkgcode not set bro damn
}

int tbwgcon1SendPackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;
	*h = header;
	send(socket_fd, memptr, /*someway to instert package code here*/, 0);
}

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);