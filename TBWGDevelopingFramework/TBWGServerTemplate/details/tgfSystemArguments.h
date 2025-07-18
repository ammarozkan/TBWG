void tgfSystemArguments(int argc, char*argv[])
{
	int getip = 0, getport = 0, getrounds = 0;
	for (unsigned int i = 0 ; i < argc ; i += 1) {
		if ( strcmp(argv[i], "--ip") == 0 ) getip = 1;
		else if (strcmp(argv[i], "--port") == 0) getport = 1;
		else if (strcmp(argv[i], "--rounds") == 0) getrounds = 1;
		else if (getport == 1) {
			unsigned long int portp = strtoul(argv[i], NULL, 0);
			port = (uint16_t)(portp);
			getport = 0;
		}
		else if (getip == 1) {
			getip = 0;
			ip_addr = argv[i];
		} else if (getrounds == 1) {
			getrounds = 0;
			rounds = (unsigned int)strtoul(argv[i], NULL, 0);
		}

	}
}