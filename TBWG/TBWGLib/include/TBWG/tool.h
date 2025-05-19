
#define TOOL_NONE 0x00
#define TOOL_HAND 0x01
#define TOOL_STICK 0x02
#define TOOL_NONCODED 0x99

#define TOOL_SPEC_TOOLY (1<<1)
#define TOOL_SPEC_KINEFY (1<<2)
#define TOOL_SPEC_THICKSTICKY (1<<3)


struct Tool {
	unsigned int toolCode;
	char* name;
	int sharpness;
};