#include <TBWG_net/tbwgcon1.h>
#include <unistd.h> // fork

#include <stdio.h>

#define REFERENCE_CHARACTER_CODE 32 // this is the character code that is needed to pass to client

char* svnm = "SV";
char* clnm = "CL";

char* putname;

void itssv(){putname=svnm;}
void itscl(){putname=clnm;}

//
//
//			SERVER TEST
//
//

int failure(char* text, int val, int req)
{
	char res[128];
	sprintf(res, "%s:%s (%i)\n\0", putname, text, req);
	int err = (req == 0 && val <= 0) || (req != 0 && val != req);
	if (err) printf(res,val);
	return err;
}

void testprint(char* text)
{
	printf("%s:%s\n",putname,text);
}

/*
struct TBWGConMidCharacterInformation testingcharacterdecider(void* ptr)
{
	iValue t = {.value = 5, .max = 5};
	struct TBWGConCharacterInformation inf = {REFERENCE_CHARACTER_CODE, t, t, t};
	struct TBWGConMidCharacterInformation result = {.inf = inf, .systematicPtr = NULL};
	return result;
}
*/

uint8_t CharacterSelector(struct TBWGConCharacterInformator cinfer)
{
selectacharacter:
	printf("SELECT A CHARACTER:\n");
	for(unsigned int i = 0 ; i < cinfer.characterCount ; i += 1) {
		struct TBWGConCharacterInformation inf = cinfer.charinfo[i];
		printf("%u:Character[%u]\n",i,inf.code);
	}
	unsigned int selection = 0;
	//scanf("%u",selection);
	if (selection < cinfer.characterCount) return selection;
	else {
		printf("Beyond limit.\n");
		goto selectacharacter;
	}
}



int accepttest(int svsock)
{
	printf("\n\n\n");
	testprint("Trying to accept!");

	struct List charinfoList = createList();
	struct TBWGConCharacterInformation examplechar = {.code = 32};

	struct TBWGConPtsizedCharacterInformationListElement charinfelm = {.charinf.inf = examplechar, .charinf.systematicPtr = NULL};
	addElement(&charinfoList, (void*)&charinfelm, sizeof(struct TBWGConPtsizedCharacterInformationListElement));

	struct TBWGConServerResult res = tbwgcon1Accept(svsock, charinfoList,NULL);
	printf("FROMSERVER: NAME IS %s and CHARACTER IS %u!\n", res.name, res.midinf.inf.code);
	int cl_fd = res.socket;

	if (cl_fd == -1) {
		printf("SV:Non cool request!\n");
	} else if (cl_fd == -2) {
		printf("SV:Non cool version!\n");
	}

	int ret = 1;
	if (cl_fd < 0) ret = cl_fd;
	tbwgcon1Close(cl_fd);
	return ret;
}

int servertest()
{
	putname = svnm;
	int erv = 0;

	testprint("Initting global recv ptr for server.");
	tbwgcon1InitGlobalRecvPtr();

	testprint("Initializing the inertia tensor.");
	TbwgConSocket sck = tbwgcon1GetProperServerSocket("127.0.0.1", 5045);

	if(failure("Server socket generation failed with %i", sck, 0)) return -1;

	testprint("Aight.");

	if (failure("Accept test failure with %i", accepttest(sck), 0)) return -1;
	testprint("Did good.");
	
	testprint("Start again for other one.");
	sleep(1);

	if (failure("Accept test failure with %i", accepttest(sck), -64)) return -1;

	testprint("Good one.");
	sleep(1);
	return 1000;
}


//
//
//		CLIENT TEST
//
//

int connecttest()
{
	testprint("Going to connect.");
	char* name = "John";
	struct TBWGConClientResult res = tbwgcon1Connect("127.0.0.1", 5045,name,CharacterSelector);
	printf("FROMCLIENT: NAME IS %s and CHARACTER IS %u!\n", name, res.inf.code);
	if (res.socket == -2) return -2;
	if (failure("Dang whats that character with %i", res.inf.code, REFERENCE_CHARACTER_CODE)) return -22;

	int sck = res.socket;

	testprint("Is it sick?");
	if(failure("Dang it is indeed with %i", sck, 0)) return sck;

	testprint("Aight. Closing.");

	tbwgcon1Close(sck);
	testprint("Closed.");
	return 1;
}

int clienttest()
{
	sleep(1);
	testprint("Initting global recv ptr for client.");
	tbwgcon1InitGlobalRecvPtr();

	if (failure("Connect test failure with %i\n", connecttest(), 0)) return -1;

	testprint("-------------------Now trying with a different header...");

	struct TBWGConHeader header = {.tbwgname = {'a','b','1','2'}, .version = {0,0,0}};
	tbwgcon1SetHeader(header);
	

	testprint("Trying to connect again.");
	if (failure("Connect test failure with %i\n", connecttest(), -2)) return -1;

	tbwgcon1SetDefaultHeader();

	testprint("Whats going on LOL.");
	sleep(1);

	return 0;
}




int main()
{
	int res = 0;
	if (fork() == 0) {
		itssv();
		res = servertest();
	} else {
		itscl();
		res = clienttest();
	}

decide:
	if(res == -1) testprint("fail.");
	else testprint("SUCCESS!");
	return 0;
}
