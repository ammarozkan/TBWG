
#define TOOL_NONE 0x00
#define TOOL_HAND 0x01
#define TOOL_STICK 0x02

struct Tool {
	unsigned int toolCode;
	int sharpness;
	int toughness;
};