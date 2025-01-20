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


int tbwgcon1SetDefaultHeader()
{
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	return tbwgcon1SetHeader(header);
}


struct TBWGConHeader tbwgcon1GetHeader(uint8_t pkgcode)
{
	struct TBWGConHeader h = header;
	h.pkgcode = pkgcode;
	return h;
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
#include <errno.h>
int tbwgcon1ReceivePackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	// this function should see the pkgcode not set bro damn // wait it actually makes sense my fault
	// saooo sage to forgot. saaauusage. got it? sousage coder. my b

	int readen = recv(socket_fd,memptr,4096,0);
	if (readen == 0 || (readen == -1 && errno == 9)) return -2;

	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;

	if (h->pkgcode == pkgcode) return 1;

	return 0;
}

int tbwgcon1SendPackage(int socket_fd, void* memptr, uint8_t pkgcode, size_t size)
{
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;
	*h = header;
	h->pkgcode = pkgcode;
	return send(socket_fd, memptr, size, 0) == size;
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

struct TBWGConCheckingPackage getNearest(struct TBWGConCheckingPackage* cpkg)
{
	struct TBWGConCheckingPackage near_cpkg = *(struct TBWGConCheckingPackage*)&header;
	return near_cpkg;
}

struct TBWGConServerResult tbwgcon1GetSvError(int err)
{
	struct TBWGConServerResult r = {.socket = err};
	return r;
}

struct TBWGConServerResult tbwgcon1Accept(int sv_fd, struct List characterList, tbwgcon1CharacterDecider charDecider, void* decidersptr)
{
	DEBUG_PRINT("tbwgcon1Accept","begin");
	struct sockaddr_in addr;
	socklen_t size = (socklen_t)sizeof(addr);

	int cl_fd = accept(sv_fd, (struct sockaddr*)&addr, &size);
	int wrongversion = 0;

chapter1:
	if (wrongversion == 2) {
		close(cl_fd);
		return tbwgcon1GetSvError(-4); // wrong version twice
	}

	if (!justread(cl_fd, GLB_RECV, sizeof(struct TBWGConCheckingPackage))) {
		close(cl_fd);
		return tbwgcon1GetSvError(-1); // non-cool connection request
	}
	DEBUG_PRINT("tbwgcon1Accept","checking package readed");

	struct TBWGConWelcomingPackage pkg;
	pkg.ip = *(struct TBWGConCheckingPackage*)GLB_RECV;
	pkg.errcode = 0;
	pkg.nextchapter = 2;

	if( tbwgcon1HeaderCheck(*(struct TBWGConHeader*)GLB_RECV) != 0 ) { // header and the checking thing has the same start
		DEBUG_PRINT("tbwgcon1Accept","header check incorrect lets send the nearest supported one");
		pkg.ip = getNearest((struct TBWGConCheckingPackage*)GLB_RECV);
		pkg.errcode = 0; pkg.nextchapter = 1;
		if(!justsend(cl_fd, &pkg, sizeof(struct TBWGConWelcomingPackage))) {
			close(cl_fd);
			return tbwgcon1GetSvError(-2);
		}
		wrongversion += 1;
		goto chapter1;
	}

	DEBUG_PRINT("tbwgcon1Accept","checking package is okay");

	if(!justsend(cl_fd, &pkg, sizeof(struct TBWGConWelcomingPackage))) {
		close(cl_fd);
		return tbwgcon1GetSvError(-3); // non-cool sent of checking package
	}

	DEBUG_PRINT("tbwgcon1Accept","Welcoming Package sent");

chapter2: // FINALLY we can use Receive and Send Package funcs from tbwgcon1

	DEBUG_PRINT("tbwgcon1Accept","Receiving the introducement pkg.");

	int r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_INTRODUCEMENTPACKAGE);
	if (r == -1) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Accept", "Nothing came up from receiving.");
		return tbwgcon1GetSvError(-4);
	} else if(r == 0) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Accept", "Unexpected package receivement.");
		return tbwgcon1GetSvError(-5);
	}

	struct TBWGConIntroducementPackage* intrd = GLB_RECV;
	size_t namesize = tbwgstrlen(intrd->name) + 1;
	char* name = tbwgmalloc(namesize);
	tbwgmemcpy(name, intrd->name, namesize);

#ifdef TBWG_DEBUG
	printf("THE NAME IS %s!!!!!!\n", intrd->name);
#endif

	DEBUG_PRINT("tbwgcon1Accept", "Introducement package is good! Really gooooodddd!!!!");

	DEBUG_PRINT("tbwgcon1Accept","Introducement Response is going up.");

	struct TBWGConIntroducementResponse intrdresp = {.errcode = 0, .nextchapter = 10}; // im not doing chapter 3 for now.
	if (!tbwgcon1SendPackage(cl_fd, &intrdresp, TBWGCON1_INTRODUCEMENTRESPONSE, sizeof(intrdresp))) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Accept","Damn introducement response can't be sended.");
		return tbwgcon1GetSvError(-6);
	}


chapter3:
	DEBUG_PRINT("tbwgcon1Accept","Entering to chapter3!");
	goto chapter10;

chapter10:
	DEBUG_PRINT("tbwgcon1Accept","Entering to chapter10!");
	struct TBWGConWait ch10wait;
	if (!tbwgcon1SendPackage(cl_fd, &ch10wait, TBWGCON1_WAIT, sizeof(ch10wait))) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Accept","Wait can't be sended.");
		return tbwgcon1GetSvError(-101);
	}
	DEBUG_PRINT("tbwgcon1Accept","Wait sent!");

	struct TBWGConMidCharacterInformation midinfo = charDecider(decidersptr);
	struct TBWGConNewCharacterInfo itsinfo = {.charinfo = midinfo.inf};
	if (!tbwgcon1SendPackage(cl_fd, &itsinfo, TBWGCON1_NEWCHARACTERINFO, sizeof(itsinfo))) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Accept", "New character info can't be sended.");
		return tbwgcon1GetSvError(-102);
	}
	DEBUG_PRINT("tbwgcon1Accept","New character info sent!");

	goto tbwgconsurepart;

tbwgconsurepart:
	
	struct TBWGConServerResult serverres = {.socket = cl_fd, .name = name, .midinf = midinfo};
	return serverres;
}


struct TBWGConClientResult tbwgcon1GetClError(int errcode)
{
	struct TBWGConClientResult r = {.socket = errcode};
	return r;
}

struct TBWGConClientResult tbwgcon1Connect(char* ip_c, uint16_t port, char* name)
{
	int r; // will be used in result of returns

	DEBUG_PRINT("tbwgcon1Connect","begin");
	int cl_fd = tbwgcon1GetProperClientSocket(ip_c, port);

chapter1:
	DEBUG_PRINT("tbwgcon1Connect", "chapter1");

	if(!justsend(cl_fd, &header, sizeof(struct TBWGConCheckingPackage))) {
		close(cl_fd);
		return tbwgcon1GetClError(-1); // checking package couldnt be sent
	}
	DEBUG_PRINT("tbwgcon1Connect","checking package sent!");

	if(!justread(cl_fd, GLB_RECV, sizeof(struct TBWGConWelcomingPackage))) {
		close (cl_fd);
		return tbwgcon1GetClError(-2); // theres no welcoming package to process.
	}
	DEBUG_PRINT("tbwgcon1Connect","welcoming package readen!");

	struct TBWGConWelcomingPackage* pkg = GLB_RECV;
	if (pkg->nextchapter == 2) goto chapter2; // YEAAAH!
	else if (pkg->nextchapter == 1) goto chapter1;
	else return tbwgcon1GetClError(-3); // sad.

chapter2:
	
	DEBUG_PRINT("tbwgcon1Connect","introducement package will be sent.");
	struct TBWGConIntroducementPackage intrd = {.header = tbwgcon1GetHeader(TBWGCON1_INTRODUCEMENTPACKAGE), .nameSize = tbwgstrlen(name)};
	tbwgmemcpy(intrd.name, name, tbwgstrlen(name)+1);
	if(!justsend(cl_fd, &intrd, sizeof(struct TBWGConIntroducementPackage))) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Connect","introducement package had an error while sending.");
		return tbwgcon1GetClError(-4); // introducement package couldnt be sent
	}
	DEBUG_PRINT("tbwgcon1Connect","okay. now response is being waited.\n");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_INTRODUCEMENTRESPONSE);
	if (r == -1) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Connect", "Nothing came up from receiving response.");
		return tbwgcon1GetClError(-5);
	} else if(r == 0) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Connect", "Unexpected package receivement of response.");
		return tbwgcon1GetClError(-6);
	}

	DEBUG_PRINT("tbwgcon1Connect","Introducement Response readed well.");

	struct TBWGConIntroducementResponse* intrdresp = GLB_RECV;

	DEBUG_PRINT("tbwgcon1Connect","If statements on the introducement response.");

	if (intrdresp->nextchapter == 3) goto chapter3;
	if (intrdresp->nextchapter == 10) goto chapter10;
	else if(intrdresp->nextchapter == 2) goto chapter2;
	else if(intrdresp->nextchapter == 1) goto chapter1;
	else if(intrdresp->errcode != 0) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Connect","DAMN. Introducement response rejected us...");
		return tbwgcon1GetClError(-7);
	}

	DEBUG_PRINT("tbwgcon1Connect","Everything is good. Entering to chapter3.");

chapter3:
	goto tbwgconsurepart;

chapter10:
	DEBUG_PRINT("tbwgcon1Connect","Eeeeyy chapter10 man!");

chapter10pkgretr:

	DEBUG_PRINT("tbwgcon1Connect","Reading package for chapter10");	
	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_NEWCHARACTERINFO);
	if (r == -1) {
		close(cl_fd);
		DEBUG_PRINT("tbwgcon1Connect", "Nothing came up from receiving response. (newcharinf)");
		return tbwgcon1GetClError(-105);
	} else if(r == 0) {
		close(cl_fd);
		if (((struct TBWGConHeader*)GLB_RECV)->pkgcode == TBWGCON1_WAIT) goto chapter10pkgretr;
		DEBUG_PRINT("tbwgcon1Connect", "Unexpected package receivement of response. (newcharinf)");
		return tbwgcon1GetClError(-106);
	}
	struct TBWGConNewCharacterInfo info = *(struct TBWGConNewCharacterInfo*)GLB_RECV;
	printf("DACIHAHIEIJGDLJAKGKLJDAKLJGDKLJALGKJ %u",info.charinfo.code);
	goto tbwgconsurepart;

tbwgconsurepart:
	DEBUG_PRINT("tbwgcon1Connect","Connection sure?");

	struct TBWGConClientResult clientres = {.socket = cl_fd, .inf = info.charinfo};
	return clientres;
}


int tbwgcon1Close(int sock)
{
	return close(sock);
}