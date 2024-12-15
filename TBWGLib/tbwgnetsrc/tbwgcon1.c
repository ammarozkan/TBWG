#include <TBWG_net/tbwgcon1.h>
#include <TBWG/essentials.h>
#include <sys/socket.h> // read, write

int pkgcodesize[64] = {};

struct TBWGConHeader header;

unsigned int getpkgsize(uint8_t pkgcode)
{
	switch(pkgcode) {
	case 0: return sizeof(struct TBWGConCheckingPackage);
	case 1: return sizeof(struct TBWGConWelcomingPackage);
	case 2: return sizeof(struct TBWGConIntroducementPackage);
	case 3: return sizeof(struct TBWGConIntroducementResponse);
	case 4: return sizeof(struct TBWGConCharacterInformator);
	case 5: return sizeof(struct TBWGConCharacterSelection);
	case 6: return sizeof(struct TBWGConCharacterSelectionError);
	case 7: return sizeof(struct TBWGConWait);
	case 8: return sizeof(struct TBWGConNewCharacterInfo);
	case 200: return sizeof(struct TBWGConSure);
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

	int readen = recv(socket_fd,memptr,4096,0);
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

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader h)
{
	uint32_t size = sizeof(struct TBWGConObservingInformation) - sizeof(void*)*(EFFECT_TRIGGER_TYPE_COUNT+3); 
															   //ignoring ptrs that in the struct

	for (unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		size += h.effectCounts[i] * sizeof(struct TBWGConEffectInformation);
	}
	size += h.eventerCount * sizeof(struct TBWGConEventerInformation);
	size += h.characterInformationCount * sizeof(struct TBWGConCharacterInformation);
	size += h.entityInformationCount * sizeof(struct EntityInformation);

	return size;
}

uint32_t tbwgcon1GetEventerOptionsInformationSize(struct TBWGConEventerOptionsInformationHeader h)
{
	uint32_t size = sizeof(struct TBWGConEventerOptionsInformation) - sizeof(void*);
																	//ignoring ptrs that in the struct
	size += h.eventerCount * sizeof(struct TBWGConEventerInformation);

	return size;
}