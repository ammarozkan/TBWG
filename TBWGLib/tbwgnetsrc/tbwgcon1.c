#include <TBWG_net/tbwgcon1.h>
#include <TBWG/essentials.h>
#include <sys/socket.h> // read, write
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr
#include <unistd.h>

#ifdef TBWG_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(func,txt) printf("DEBUG:%s %s\n",func,txt)
#else
#define DEBUG_PRINT(x,y)
#endif /*TBWG_DEBUG*/

#define GLB_RECV globalrecvptr

void* globalrecvptr;

struct TBWGConHeader header;

void tbwgcon1InitGlobalRecvPtr()
{
	globalrecvptr = malloc(2048);
}


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

int tbwgcon1HeaderCheck(struct TBWGConHeader h)
{
	for (unsigned int i = 0 ; i < 4 ; i += 1) {
		if (h.tbwgname[i] != header.tbwgname[i]) return -1;
	} for(unsigned int i = 0 ; i < 3 ; i += 1) {
		if (h.version[i] != header.version[i]) return -2;
	}

	return 0;
}

int tbwgcon1GetProperServerSocket(char* ip_c, uint16_t port)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd <= 0)
	{
		return -1;
	}

	int okey = 1;
	if(setsockopt(server_fd,
		SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&okey,
		sizeof(okey)) < 0)
	{
		return -2;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_c);
	socklen_t size = (socklen_t)sizeof(addr);
	if(bind(server_fd,(struct sockaddr*)&addr,size) < 0)
	{
		return -3;
	}
	listen(server_fd, 4);
	return server_fd;
}

int tbwgcon1GetProperClientSocket(char* ip_c, uint16_t port)
{
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd <= 0)
	{
		return -1;
	}

	int okey = 1;
	if(setsockopt(client_fd,
		SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&okey,
		sizeof(okey)) < 0)
	{
		return -2;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_c); // idk look to that inf.
	socklen_t size = (socklen_t)sizeof(addr);
	if(connect(client_fd,(struct sockaddr*)&addr,size) < 0)
	{
		return -3;
	}

	return client_fd;
}

int tbwgcon1ReceivePackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	// this function should see the pkgcode not set bro damn // wait it actually makes sense my fault
	// saooo sage to forgot. saaauusage. got it? sousage coder. my b

	int readen = recv(socket_fd,memptr,4096,0);
	if (readen == 0) return -2;

	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;

	if (h->pkgcode == pkgcode) return 1;

	return 0;
}

int tbwgcon1SendPackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;
	*h = header;
	h->pkgcode = pkgcode;
	send(socket_fd, memptr, getpkgsize(pkgcode), 0);

	return 0;
}


int tbwgcon1ReceiveCheckingPackage(int socket_fd, struct TBWGConCheckingPackage* memptr)
{
	int readen = recv(socket_fd, memptr, sizeof(struct TBWGConCheckingPackage), 0);

	if (readen != sizeof(struct TBWGConCheckingPackage)) return 0;
	else return 1;
}

int tbwgcon1SendWelcomingPackage(int socket_fd, struct TBWGConWelcomingPackage* pkg)
{
	int sent = send(socket_fd, pkg, sizeof(struct TBWGConWelcomingPackage), 0);
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


int justread(int socket_fd, void* mem, size_t size)
{
	int readen = recv(socket_fd, mem, size, 0);
	if (readen != size) return 0;
	else return 1;
}

int justsend(int socket_fd, void* mem, size_t size)
{
	int sent = send(socket_fd, mem, size, 0);
	if (sent != size) return 0;
	else return 1;
}

int tbwgcon1Accept(int sv_fd)
{
	DEBUG_PRINT("tbwgcon1Accept","begin");
	struct sockaddr_in addr;
	socklen_t size = (socklen_t)sizeof(addr);

	int cl_fd = accept(sv_fd, (struct sockaddr*)&addr, &size);

	if (!justread(cl_fd, GLB_RECV, sizeof(struct TBWGConCheckingPackage))) {
		close(cl_fd);
		return -1; // non-cool connection request
	}
	DEBUG_PRINT("tbwgcon1Accept","checking package readed");

	if( tbwgcon1HeaderCheck(*(struct TBWGConHeader*)GLB_RECV) != 0) { // header and the checking thing has the same start
		close(cl_fd);
		return -2; // non-cool header
	}
	DEBUG_PRINT("tbwgcon1Accept","checking package is okay");

	struct TBWGConWelcomingPackage pkg;
	pkg.ip = *(struct TBWGConCheckingPackage*)GLB_RECV;
	pkg.errcode = 0;
	pkg.nextchapter = 2;

	if(!justsend(cl_fd, &pkg, sizeof(struct TBWGConWelcomingPackage))) {
		close(cl_fd);
		return -3; // non-cool sent of checking package
	}

	DEBUG_PRINT("tbwgcon1Accept","Welcoming Package sent");

	return cl_fd;
}

int tbwgcon1Connect(char* ip_c, uint16_t port)
{
	DEBUG_PRINT("tbwgcon1Connect","begin");
	int cl_fd = tbwgcon1GetProperClientSocket(ip_c, port);

	if(!justsend(cl_fd, &header, sizeof(struct TBWGConCheckingPackage))) {
		close(cl_fd);
		return -1; // checking package couldnt be sent
	}
	DEBUG_PRINT("tbwgcon1Connect","checking package sent!");

	if(!justread(cl_fd, GLB_RECV, sizeof(struct TBWGConWelcomingPackage))) {
		close (cl_fd);
		return -2; // theres no welcoming package to process.
	}
	DEBUG_PRINT("tbwgcon1Connect","welcoming package readen!");

	struct TBWGConWelcomingPackage* pkg = GLB_RECV;
	if (pkg->nextchapter == 2) return 1; // YEAAAH!
	else return -3; // sad.
}


int tbwgcon1Close(int sock)
{
	return close(sock);
}