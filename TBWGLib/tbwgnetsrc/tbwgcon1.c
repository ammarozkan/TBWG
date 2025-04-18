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

struct TBWGConHeader getNearest(struct TBWGConHeader* cpkg)
{
	struct TBWGConHeader near_cpkg = header;
	return near_cpkg;
}

struct TBWGConServerResult tbwgcon1GetSvError(int err)
{
	struct TBWGConServerResult r = {.socket = err};
	return r;
}


struct TBWGConClientResult tbwgcon1GetClError(int errcode)
{
	struct TBWGConClientResult r = {.socket = errcode};
	return r;
}

#define TBWGCON1_BROKEUPSERVERWITHERROR(str,errcode) {close(cl_fd);DEBUG_PRINT("tbwgcon1Accept",str);return tbwgcon1GetSvError(errcode);}

struct TBWGConServerResult tbwgcon1Accept(int sv_fd, struct List characterList, tbwgcon1CharacterDecider charDecider, void* decidersptr)
{
	DEBUG_PRINT("tbwgcon1Accept","begin");
	struct TBWGConPtsizedCharacterInformation midinfo = {.systematicPtr = NULL}; char name[32]; int cl_fd = 0; int r;
	DEBUG_PRINT("tbwgcon1Accept","accepting");
	struct sockaddr_in addr;
	socklen_t size = (socklen_t)sizeof(addr);

	cl_fd = accept(sv_fd, (struct sockaddr*)&addr, &size);
	DEBUG_PRINT("tbwgcon1Accept","accepted!");

	unsigned int chapter1counter = 0;

chapter1:
	chapter1counter += 1;
	if (chapter1counter >= 3) TBWGCON1_BROKEUPSERVERWITHERROR("Tried too many times on chapter 1. B-L-U-N-D-E-R-E-D",-64);
	DEBUG_PRINT("tbwgcon1Accept","chapter1!");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_ENTERINGPACKAGE);
	if (r == -1) TBWGCON1_BROKEUPSERVERWITHERROR("Nothing came up from receiving.",-1)
	else if(r == 0) TBWGCON1_BROKEUPSERVERWITHERROR("unexpected package receivement.",-2);
	DEBUG_PRINT("tbwgcon1Accept","Entering Package OK!");

	struct TBWGConEnteringResponse ent_resp = {.errcode = 0, .nextchapter = 2};
	struct TBWGConEnteringPackage ent = *(struct TBWGConEnteringPackage*)GLB_RECV;

	if(tbwgcon1HeaderCheck(ent.header) != 0) {
		ent_resp.errcode |= TBWGCON1_ERR_UNSUPPORTEDVERSION;
		ent_resp.nextchapter = 1;
		DEBUG_PRINT("tbwgcon1Accept","HEADER CHECK FAIL, NEXT IS 1 AND ERROR CODE ADDED!");
	}

	DEBUG_PRINT("tbwgcon1Accept","EnteringResponse sending!");
	tbwgcon1SendPackage(cl_fd, (void*)&ent_resp, TBWGCON1_ENTERINGRESPONSE, sizeof(ent_resp));

	if (ent_resp.nextchapter == 1) goto chapter1;
	else if (ent_resp.nextchapter == 2) goto chapter2;
	else TBWGCON1_BROKEUPSERVERWITHERROR("chapter 1, WHERE THE F@?* SHOULD SERVER GO?",-3);

chapter2:
	DEBUG_PRINT("tbwgcon1Accept","chapter 2!");

	struct TBWGConCharacterInformator cinfer = {.characterCount = 0};

	DEBUG_PRINT("tbwgcon1Accept","from available character list, generating the infos.");
	ITERATE(characterList, charlistElm_pure) {
		struct TBWGConCharacterInformation charinf = ((struct TBWGConPtsizedCharacterInformationListElement*)charlistElm_pure)->charinf.inf;

		cinfer.charinfo[cinfer.characterCount] = charinf;
		cinfer.characterCount += 1; if (cinfer.characterCount == TBWGCON1_MAX_AVAILABLE_CHARACTER_COUNT) break;
	}

	DEBUG_PRINT("tbwgcon1Accept","generation completed. sending the character informator.");
	tbwgcon1SendPackage(cl_fd, (void*)&cinfer, TBWGCON1_CHARACTERINFORMATOR, sizeof(cinfer));
	DEBUG_PRINT("tbwgcon1Accept","character informator sent!");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_CHARACTERSELECTION);
	if (r == -1) TBWGCON1_BROKEUPSERVERWITHERROR("Nothing came up from receiving.",-4)
	else if(r == 0) TBWGCON1_BROKEUPSERVERWITHERROR("unexpected package receivement.",-5);
	DEBUG_PRINT("tbwgcon1Accept","character selection receivement OK!");

	struct TBWGConCharacterSelection charsl = *(struct TBWGConCharacterSelection*)GLB_RECV;

	struct TBWGConCharacterSelectionError charsl_err = {.errcode = 0};

	if (charsl.selection >= cinfer.characterCount) {
		charsl_err.errcode |= TBWGCON1_CHARSELECTERR_CHOOSEDABOVE;
		DEBUG_PRINT("tbwgcon1Accept","CLIENT SELECTED ABOVE THE LIMIT.");
	}

	if (charsl_err.errcode != 0) {
		charsl_err.nextchapter = 2;
		DEBUG_PRINT("tbwgcon1Accept","THERES AN ERROR ON THE SELECTION. NEXT IS 2");
	}

	DEBUG_PRINT("tbwgcon1Accept","selection error OK sending.");
	tbwgcon1SendPackage(cl_fd,(void*)&charsl_err,TBWGCON1_CHARACTERSELECTIONERROR,sizeof(charsl_err));
	DEBUG_PRINT("tbwgcon1Accept","selection error sent!");

	if (charsl_err.nextchapter == 2) goto chapter2;
	else if(charsl_err.nextchapter == 3) goto chapter3;

chapter3:


	// GETTING THE SYSTEMATIC POINTER OF THE CHOOSEN CHARACTER SOMEHOW
	unsigned int ccc = 0;
	ITERATE(characterList, charlistElm_pure) {
		struct TBWGConCharacterInformation charinf = ((struct TBWGConPtsizedCharacterInformationListElement*)charlistElm_pure)->charinf.inf;

		if (ccc == charsl.selection) midinfo = ((struct TBWGConPtsizedCharacterInformationListElement*)charlistElm_pure)->charinf;
		ccc += 1; if (ccc == TBWGCON1_MAX_AVAILABLE_CHARACTER_COUNT) break;
	}

	DEBUG_PRINT("tbwgcon1Accept","chapter 3 and end.");
	struct TBWGConServerResult serverres = {.socket = cl_fd, .name = malloc(TBWGCON1_STD_NAME_SIZE), .midinf = midinfo};
	tbwgmemcpy(serverres.name,ent.name,TBWGCON1_STD_NAME_SIZE);
	return serverres;
}

#define TBWGCON1_BROKEUPCLIENTWITHERROR(str,errcode) { close(cl_fd); DEBUG_PRINT("tbwgcon1Connect", str); return tbwgcon1GetClError(errcode); }

struct TBWGConClientResult tbwgcon1Connect(char* ip_c, uint16_t port, char* name)
{
	int cl_fd = 0; struct TBWGConCharacterInformation info; int r;

	DEBUG_PRINT("tbwgcon1Connect","getting proper client socket!");

	cl_fd = tbwgcon1GetProperClientSocket(ip_c, port);

	DEBUG_PRINT("tbwgcon1Connect","OK?");

chapter1:
	DEBUG_PRINT("tbwgcon1Connect","chapter1!");
	struct TBWGConEnteringPackage ent = {.namesize = tbwgstrlen(name)};
	tbwgmemcpy(ent.name,name,tbwgstrlen(name));
	tbwgcon1SendPackage(cl_fd, (void*)&ent, TBWGCON1_ENTERINGPACKAGE, sizeof(ent));

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_ENTERINGRESPONSE);
	if (r == -1) TBWGCON1_BROKEUPCLIENTWITHERROR("Nothing came up from receiving.",-1)
	else if(r == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("unexpected package receivement.",-2);
	DEBUG_PRINT("tbwgcon1Connect","Entering Response OK!");

	struct TBWGConEnteringResponse ent_resp = *(struct TBWGConEnteringResponse*)GLB_RECV;

	//printf("NCP:%u\n",ent_resp.nextchapter);

	if (ent_resp.nextchapter == 1) goto chapter1;
	else if(ent_resp.nextchapter == 2) goto chapter2;
	else if (ent_resp.errcode != 0) TBWGCON1_BROKEUPCLIENTWITHERROR("SERVER RETURNED AN ERROR",-3)
	else TBWGCON1_BROKEUPCLIENTWITHERROR("chapter 1, WHERE THE F@?* SHOULD CLIENT GO?",-4);

chapter2:
	DEBUG_PRINT("tbwgcon1Connect","chapter 2!");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_CHARACTERINFORMATOR);
	if (r == -1) TBWGCON1_BROKEUPCLIENTWITHERROR("Nothing came up from receiving.",-5)
	else if(r == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("unexpected package receivement.",-6);
	DEBUG_PRINT("tbwgcon1Connect","Character Informator OK!");
	struct TBWGConCharacterInformator cinfer = *(struct TBWGConCharacterInformator*)GLB_RECV;

	//printf("CLIENT READS %u CHARACTER INFOS.\n",cinfer.characterCount);

	if (cinfer.characterCount == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("not enough characters to choose.",-7);


	DEBUG_PRINT("tbwgcon1Connect","Generating character selection!");
	struct TBWGConCharacterSelection charsl = {.selection = 0, .options = 0};

	DEBUG_PRINT("tbwgcon1Connect","Sending character selection!");
	tbwgcon1SendPackage(cl_fd, (void*)&charsl, TBWGCON1_CHARACTERSELECTION, sizeof(charsl));
	DEBUG_PRINT("tbwgcon1Connect","Character selection sent!");

	info = cinfer.charinfo[charsl.selection];

	struct TBWGConClientResult clientres = {.socket = cl_fd, .inf = info};
	return clientres;
}


int tbwgcon1Close(int sock)
{
	return close(sock);
}