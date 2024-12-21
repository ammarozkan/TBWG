#include <TBWG_net/tbwgcon1.h>
#include <unistd.h> // fork

#include <stdio.h>

//
//
//			SERVER TEST
//
//

int servertest()
{
	printf("SV:Initting global recv ptr for server.\n");
	tbwgcon1InitGlobalRecvPtr();

	printf("SV:Initializing the inertia tensor.\n");
	int sck = tbwgcon1GetProperServerSocket("127.0.0.1", 5045);

	printf("SV:Is it sick?\n");
	if (sck <= 0) {
		printf("SV:Dang it is indeed with %i.\n", sck);
		return sck;
	} //

	printf("SV:Aight.\n");

	printf("SV:Trying to accept!\n");

	int cl_fd = tbwgcon1Accept(sck);

	if (cl_fd == -1) {
		printf("SV:Non cool request!\n");
	} else if (cl_fd == -2) {
		printf("SV:Non cool version!\n");
	}

	printf("SV:We're doing good. Closing.\n");
	//tbwgcon1Close(cl_fd);
}


//
//
//		CLIENT TEST
//
//

int connecttest()
{
	printf("CL:Going to connect.\n");
	int sck = tbwgcon1Connect("127.0.0.1", 5045);

	printf("CL:Is it sick?\n");
	if (sck <= 0) {
		printf("CL:Dang it is indeed with %i\n",sck);
		return sck;
	}

	printf("CL:Aight. Closing.\n");
	//tbwgcon1Close(sck);
	printf("CL:Closed.\n");
	return 0;
}

int clienttest()
{
	sleep(1);
	printf("CL:Initting global recv ptr for client.\n");
	tbwgcon1InitGlobalRecvPtr();

	if(connecttest() == 0) printf("CL:No Problem 1!\n");

	printf("CL:Now trying with a different header...\n");

	struct TBWGConHeader header = {.tbwgname = {'a','b','1','2'}, .version = {0,0,0}};
	tbwgcon1SetHeader(header);

	printf("CL:Trying to connect again.\n");
	if(connecttest() == 0) printf("CL:No Problem 2!\n");

	printf("Whats going on LOL.\n");

	return 0;
}




int main()
{
	if (fork() == 0) servertest();
	else clienttest();
	return 0;
}
