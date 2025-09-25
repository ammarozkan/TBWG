#ifndef TBWG_TOOL_H
#define TBWG_TOOL_H


#define TOOL_NONE 0x00
#define TOOL_HAND 0x01
#define TOOL_STICK 0x02
#define TOOL_NONCODED 0x99

#define TOOL_SPEC_TOOLY (1<<1)
#define TOOL_SPEC_KINEFY (1<<2)
#define TOOL_SPEC_THICKSTICKY (1<<3)

#include <TBWG/details.h>

struct EventerRequiredInformations;
struct World;
struct Character;

struct Tool {
	unsigned int toolCode;
	int sharpness;
	float mass;
	digits32 required_informations;
	
	// tool use eventer just puts those values here
	void (*use)(struct Tool* tool, struct World*, struct Character*, 
		struct EventerRequiredInformations);
	
	Details details;
};


void defaultToolUse(struct Tool* tool, struct World*, struct Character*, 
		struct EventerRequiredInformations);

struct Tool* getDefaultTool();


#endif /*TBWG_TOOL_H*/