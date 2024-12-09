#include <TBWG_net/tbwgcon1.h>

int pkgcodesize[64] = {};

struct TBWGConHeader header;

unsigned int getpkgsize(uint8_t pkgcode)
{
	switch(pkgcode) {
	case 0: return sizeof(struct TBWGCheckingPackage);
	case 1: return sizeof(struct TBWGWelcomingPackage);
	case 2: return sizeof(struct TBWGIntroducementPackage);
	case 1: return sizeof(struct TBWGIntroducementResponse);
	case 4: return sizeof(struct TBWGCharacterInformator);
	case 5: return sizeof(struct TBWGCharacterSelection);
	case 6: return sizeof(struct TBWGCharacterSelectionError);
	case 7: return sizeof(struct TBWGWait);
	case 8: return sizeof(struct TBWGNewCharacterInfo);
	case 200: return sizeof(struct TBWGSure);
	case 33: return sizeof(struct TBWGConObservingInformationHeader);
	case 34: return sizeof(struct TBWGConObservingInformation);
	case 35: return sizeof(struct TBWGConWorldEventInformation);
	case 36: return sizeof(struct TBWGConEventerOptionsInformationHeader);
	case 37: return sizeof(struct TBWGConEventerInformation);
	case 38: return sizeof(struct TBWGConEventerOptionsInformation);
	case 39: return sizeof(struct TBWGConTurnPlay);
	}
}

int tbwgcon1SetHeader(struct TBWGConHeader h)
{
	header = h;
	return 1;
}


int tbwgcon1GetProperServerSocket(uint32_t ip, uint16_t port);
int tbwgcon1GetProperClientSocket(uint32_t ip, uint16_t port);


int tbwgcon1ReceivePackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	// this function should see the pkgcode not set bro damn // wait it actually makes sense my fault
	// saooo sage to forgot. saaauusage. got it? sousage coder. my b

	int readen = read(socket_fd,memptr,4096);
	if (readen == 0) return -2;

	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;

	return h->pkgcode == pkgcode;
}

int tbwgcon1SendPackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;
	*h = header;
	h->pkgcode = pkgcode;
	send(socket_fd, memptr, getpkgsize(pkgcode), 0);

	return 0;
}

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader);
uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader);