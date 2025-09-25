#include <TBWG_net/tbwgcon1.h>
#include <TBWG/essentials.h>

#ifdef _WIN32
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h> // read, write
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // sleep
#endif

#include <errno.h> // for errno in recv

void* globalrecvptr;

struct TBWGConHeader header;


int tbwgcon1Init()
{
	int result = 1;
#ifdef _WIN32
	WSADATA wsa;
	result = (WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif
	return result;
}

void tbwgcon1Dinit()
{
#ifdef _WIN32
	WSACleanup();
#endif
} // important for winsock


void tbwgcon1InitGlobalRecvPtr()
{
	globalrecvptr = malloc(TBWGCON1_GLOBALRECVPTR_SIZE);
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

TbwgConSocket tbwgcon1GetProperServerSocket(char* ip_c, uint16_t port)
{
	#ifdef _WIN32
	struct addrinfo *result = NULL;
    struct addrinfo hints;
	
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	const char* defaultPort = "5005";
    int iResult = getaddrinfo(NULL, defaultPort, &hints, &result);
    if ( iResult != 0 ) {
        return -1001;
    }
	TbwgConSocket server_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (!ISSOCKETOK(server_fd)) {
		return -1002;
	}
	#else
	TbwgConSocket server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	#endif
	if(!ISSOCKETOK(server_fd)) {
		return -1;
	}

	int okay = 1;
	#ifdef _WIN32
	if(setsockopt(server_fd,
		SOL_SOCKET, SO_REUSEADDR,
		(char*)&okay,
		sizeof(okay)) < 0)
	{
		return -2;
	}
	#else
	if(setsockopt(server_fd,
		SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&okay,
		sizeof(okay)) < 0)
	{
		return -2;
	}
	#endif

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_c);
	if(bind(server_fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		return -3;
	}

	listen(server_fd, 4);
	return server_fd;
}

TbwgConSocket tbwgcon1GetProperClientSocket(char* ip_c, uint16_t port)
{
	TbwgConSocket client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(ISSOCKETOK(client_fd))
	{
		return -1;
	}

	int okay = 1;
	#ifdef _WIN32
	if(setsockopt(client_fd,
		SOL_SOCKET, SO_REUSEADDR,
		(char*)&okay,
		sizeof(okay)) < 0)
	{
		return -2;
	}
	#else
	if(setsockopt(client_fd,
		SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&okay,
		sizeof(okay)) < 0)
	{
		return -2;
	}
	#endif

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_c); // idk look to that inf.
	if(connect(client_fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		return -3;
	}

	return client_fd;
}

int tbwgcon1ReceivePackage(int socket_fd, void* memptr, uint8_t pkgcode)
{
	// this function should see the pkgcode not set bro damn // wait it actually makes sense my fault
	// saooo sage to forgot. saaauusage. got it? sousage coder. my b
	DEBUG_PRINTUINT("receivePackage call","expecting!",pkgcode);
	int readen;
	struct TBWGConHeader head;
	readen = recv(socket_fd,(char*)&head,sizeof(struct TBWGConHeader),0);
	if (readen == 0) return -1;
	if (readen == -1 && errno == 9) return -2;
	// reading the header first

	void* readennewpkg = memptr+sizeof(head); // skipping head and reading the package to the pointer
	readen = recv(socket_fd,(char*)readennewpkg,head.size-sizeof(struct TBWGConHeader),0); // reading the package fully
	
	DEBUG_RECEIVEDEBUG("receivePackage call",head.size,readen+sizeof(head),pkgcode,head.pkgcode);
	if (readen == 0) return -1;
	if (readen == -1 && errno == 9) return -2;
	
	tbwgmemcpy(memptr, &head, sizeof(head)); // copying the head

	// everything is good guess featuring b
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;

	if (h->pkgcode == pkgcode) return 1;

	return 0;
}

int tbwgcon1SendPackage(int socket_fd, void* memptr, uint8_t pkgcode, uint32_t size)
{
	//sleep(1);
	struct TBWGConHeader* h = (struct TBWGConHeader*)memptr;
	*h = header;
	h->pkgcode = pkgcode;
	h->size = size;
	int sent = send(socket_fd, (char*)memptr, size, 0);
	DEBUG_SENDDEBUG("sendPackage call",size,sent,pkgcode,h->pkgcode);
	return sent > 0 && (uint32_t)sent == size;
}

uint32_t tbwgcon1GetObservingInformationSize(struct TBWGConObservingInformationHeader h)
{
	uint32_t size = sizeof(struct TBWGConObservingInformation) - sizeof(void*)*(EFFECT_TRIGGER_TYPE_COUNT+3); 
															   //ignoring ptrs that in the struct

	for (unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
		size += h.effectCounts[i] * sizeof(struct TBWGConObservingEffectInformation);
	}
	size += h.eventerCount * sizeof(struct TBWGConObservingEventerInformation);
	size += h.characterInformationCount * sizeof(struct TBWGConObservingCharacterInformation);
	size += h.entityInformationCount * sizeof(struct TBWGConObservingEntityInformation);

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

struct TBWGConServerResult tbwgcon1Accept(int sv_fd, struct List characterList, void* decidersptr)
{
	unsigned int charsl_counter = 0;


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
	if (r < 0) TBWGCON1_BROKEUPSERVERWITHERROR("Nothing came up from receiving.",-1)
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

	//sleep(7);
	DEBUG_PRINT("tbwgcon1Accept","generation completed. sending the character informator.");
	if(tbwgcon1SendPackage(cl_fd, (void*)&cinfer, TBWGCON1_CHARACTERINFORMATOR, sizeof(cinfer)) == 0) goto chapter2;
	DEBUG_PRINT("tbwgcon1Accept","character informator sent!");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_CHARACTERSELECTION);
	if (r < 0) TBWGCON1_BROKEUPSERVERWITHERROR("Nothing came up from receiving.",-4)
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




	// GETTING THE SYSTEMATIC POINTER OF THE CHOOSEN CHARACTER SOMEHOW
chapter3:
	ITERATE(characterList, charlistElm_pure) {
		struct TBWGConCharacterInformation charinf = ((struct TBWGConPtsizedCharacterInformationListElement*)charlistElm_pure)->charinf.inf;

		if (charsl_counter == charsl.selection) midinfo = ((struct TBWGConPtsizedCharacterInformationListElement*)charlistElm_pure)->charinf;
		charsl_counter += 1; if (charsl_counter == TBWGCON1_MAX_AVAILABLE_CHARACTER_COUNT) break;
	}

	DEBUG_PRINT("tbwgcon1Accept","chapter 3 and end.");
	struct TBWGConServerResult serverres = {.socket = cl_fd, .name = malloc(TBWGCON1_STD_NAME_SIZE), .midinf = midinfo};
	tbwgmemcpy(serverres.name,ent.name,TBWGCON1_STD_NAME_SIZE);
	return serverres;
}

struct TBWGConClientResult tbwgcon1Connect(char* ip_c, uint16_t port, char* name, tbwgcon1CharacterSelector charSelector)
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
	DEBUG_PRINTUINT("tbwgcon1Connect","readen should be!",sizeof(struct TBWGConEnteringResponse));
	if (r < 0) TBWGCON1_BROKEUPCLIENTWITHERROR("Nothing came up from receiving.",-1)
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
	DEBUG_PRINTUINT("tbwgcon1Connect","readen should be!",sizeof(struct TBWGConCharacterInformator));
	DEBUG_PRINT("tbwgcon1Connect","Character Informator Readed!");
	if (r < 0) TBWGCON1_BROKEUPCLIENTWITHERROR("Nothing came up from receiving.",-5)
	else if(r == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("unexpected package receivement.",-6);
	DEBUG_PRINT("tbwgcon1Connect","Character Informator OK!");
	struct TBWGConCharacterInformator cinfer = *(struct TBWGConCharacterInformator*)GLB_RECV;

	//printf("CLIENT READS %u CHARACTER INFOS.\n",cinfer.characterCount);

	if (cinfer.characterCount == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("not enough characters to choose.",-7);


	DEBUG_PRINT("tbwgcon1Connect","Generating character selection!");
	struct TBWGConCharacterSelection charsl = {.selection = charSelector(cinfer), .options = 0};

	DEBUG_PRINT("tbwgcon1Connect","Sending character selection!");
	tbwgcon1SendPackage(cl_fd, (void*)&charsl, TBWGCON1_CHARACTERSELECTION, sizeof(charsl));
	DEBUG_PRINT("tbwgcon1Connect","Character selection sent!");

	r = tbwgcon1ReceivePackage(cl_fd, GLB_RECV, TBWGCON1_CHARACTERSELECTIONERROR);
	if (r < 0) TBWGCON1_BROKEUPCLIENTWITHERROR("Nothing came up from receiving.",-8)
	else if(r == 0) TBWGCON1_BROKEUPCLIENTWITHERROR("unexpected package receivement.",-9);
	DEBUG_PRINT("tbwgcon1Connect","Character selection error OK!");

	struct TBWGConCharacterSelectionError charsl_err = *(struct TBWGConCharacterSelectionError*)GLB_RECV;

	if (charsl_err.errcode != 0) {
		DEBUG_PRINT("tbwgcon1Connect","Character selection has an error.\n");
		if (charsl_err.nextchapter == 2) goto chapter2;
		else TBWGCON1_BROKEUPCLIENTWITHERROR("Character selection error with no backchapters.",-10);
	}
	

	info = cinfer.charinfo[charsl.selection];

	struct TBWGConClientResult clientres = {.socket = cl_fd, .inf = info};
	return clientres;
}


int tbwgcon1Close(TbwgConSocket sock)
{
#ifdef _WIN32
	return closesocket(sock);
#else
	return close(sock);
#endif
}


// Essential Usage Functions


struct TBWGConEventerInformation tbwgconConvertToEventerInformation(struct Eventer e)
{
	struct TBWGConEventerInformation r;
	r.eventerCode = e.eventerCode;
	r.ID = e.ID;
	r.energy = e.baseEnergy;
	r.spellEnergy = e.baseSpellEnergy;
	r.eventer_type = e.eventer_type;
	r.required_informations = e.required_informations;
	{
		tbwgmemcpy(&(r.costs), &(e.costs), sizeof(struct EventerUses));
    }
	for(unsigned int i = 0 ; i < 8 ; i += 1) {
		r.details.details[i] = e.details[i];
	}
	return r;
}

