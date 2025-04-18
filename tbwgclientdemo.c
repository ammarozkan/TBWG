#include <stdio.h>
#include <stdlib.h>


#include <TBWG_net/tbwgcon1.h>

int main()
{
	struct TBWGConHeader header = {.tbwgname = {'t','b','w','g'}, .version = {0,0,0}};
	struct TBWGConClientResult res;
	tbwgcon1SetHeader(header);
	
	printf("globalrecvptr\n");
	tbwgcon1InitGlobalRecvPtr();

	char* name = "John";
	res = tbwgcon1Connect("127.0.0.1", 5045, name);

	if (res.socket < 0) {
		printf("Failed connection. (errcode:%i)\n",res.socket);
		return -1;
	}

	printf("GREAT!\n");

	printf("NAME IS %s and CHARACTER IS %u!\n", name, res.inf.code);
	return 0;
}