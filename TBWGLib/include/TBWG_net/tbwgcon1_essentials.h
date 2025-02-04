//	tbwgcon1_essentials.h is the file that contains structs that defined in TBWGCON1

#include <TBWG/essentials.h>
#include <TBWG/maths.h>
#include <TBWG/stats.h>
#include <TBWG/effects.h>
#include <TBWG/eventer.h>
#include <TBWG/observation.h>

#define TBWGCON1_ERR_UNSUPPORTEDVERSION (1<<0)
#define TBWGCON1_ERR_UNSUPPORTEDGAME (1<<1)
#define TBWGCON1_ERR_UNEXPECTEDPKG (1<<2)

#define TBWGCON1_CHRSLCTOPT_CREATE (1 << 0)

#define TBWGCON1_CHRSLCTERR_NOPERMISSION 0x0
#define TBWGCON1_CHRSLCTERR_ALREADYINUSE 0x1
#define TBWGCON1_VERCONTERR_TOOMANYTRIES 0x2

#define STD_NAME_SIZE 32
#define STD_EVENTNAME_SIZE STD_NAME_SIZE



struct TBWGConCharacterInformation {
    uint32_t code;
    iValue hp, e, se;
};


struct TBWGConMidCharacterInformation {
    void* systematicPtr; // a pointer to help the local tbwgmanager find the character. will be returned back to the system
    struct TBWGConCharacterInformation inf;
};

struct TBWGConServerResult {
    int socket;
    char* name;
    struct TBWGConMidCharacterInformation midinf;
};

struct TBWGConClientResult {
    int socket;
    struct TBWGConCharacterInformation inf;
};


struct TBWGConEffectInformation {
    id_number ID;
    uint32_t code;
};



struct TBWGConUsersEventerInformation {
    unsigned int eventerCode;
    id_number ID;
    uint8_t energyValueType;
    uint32_t energy, spellEnergy;
    digits32 eventer_type, required_informations;
    char name[32];
    struct EventerUses costs;
};


struct TBWGConHeader {
	char tbwgname[4];
	uint8_t version[3];
	uint8_t pkgcode;
};



/***                      TBWGCon Packages                       ***/


// pkgcode : 7
#define TBWGCON1_WAIT 7
struct TBWGConWait {
	struct TBWGConHeader header;
};

// pkgcode : 9
#define TBWGCON1_QUIT 9
struct TBWGConQuit {
    struct TBWGConHeader header;
    uint32_t errcode;
};

// pkgcode : 0
struct TBWGConCheckingPackage {
	char tbwgname[4];
	uint8_t version[3];
};

// pkgcode : 1
struct TBWGConWelcomingPackage {
	struct TBWGConCheckingPackage ip;
	uint32_t errcode;
	uint32_t nextchapter;
};

// pkgcode : 2
#define TBWGCON1_INTRODUCEMENTPACKAGE 2
struct TBWGConIntroducementPackage {
	struct TBWGConHeader header;
	uint32_t nameSize;
	char name[STD_NAME_SIZE];
};

// pkgcode : 3
#define TBWGCON1_INTRODUCEMENTRESPONSE 3
struct TBWGConIntroducementResponse {
	struct TBWGConHeader header;
	uint32_t errcode;
	uint32_t nextchapter;
};

// pkgcode : 4
struct TBWGConCharacterInformator {
	struct TBWGConHeader header;

	uint16_t characterCount;
	struct TBWGConCharacterInformation* charinfo; // UBERSTRUCTESH!
};

// pkgcode : 5
struct TBWGConCharacterSelection {
	struct TBWGConHeader header;

	uint8_t selection;
	uint8_t options; // CHARACTERSELECTION_OPTION_CREATE, etc.
};

// pkgcode : 6
struct TBWGConCharacterSelectionError {
	struct TBWGConHeader header;

	uint32_t characterSelectionErrorCode;
	uint32_t nextchapter;
};

// pkgcode : 8
#define TBWGCON1_NEWCHARACTERINFO 8
struct TBWGConNewCharacterInfo {
	struct TBWGConHeader header;
	struct TBWGConCharacterInformation charinfo;
};



// pkgcode: 200 (y)
struct TBWGConSure {
    struct TBWGConHeader header;
    char name[STD_NAME_SIZE];
    struct TBWGConCharacterInformation charinfo;
    uint8_t errcode; // should be zero if connection is okay!
};



//// ////  Playing  //// ////

// pkgcode: 33
struct TBWGConObservingInformationHeader {
    struct TBWGConHeader header;
    uint32_t effectCounts[EFFECT_TRIGGER_TYPE_COUNT];
    uint32_t eventerCount;
    uint32_t characterInformationCount;
    uint32_t entityInformationCount;
};


// pkgcode: 34
struct TBWGConObservingInformation {
    struct TBWGConHeader header;
    id_number selfid;

    struct Stats characterStats;
    iValue hp, e, se;
    iVector position; fVector direction;
    digits32 state;

    struct TBWGConEffectInformation* effects[EFFECT_TRIGGER_TYPE_COUNT]; // UBERSTRUCTESH
    struct TBWGConUsersEventerInformation* eventers; // UBERSTRUCTESH!
    struct CharacterInformation* charInfos; // UBERSTRUCTESH!
    struct EntityInformation* entityInfos; // UBERSTRUCTESH!
};

// pkgcode: 35
struct TBWGConWorldEventInformation {
    struct TBWGConHeader header;
    id_number selfid;
    char eventName[STD_EVENTNAME_SIZE];
    iVector position;
};



// pkgcode: 36
struct TBWGConEventerOptionsInformationHeader {
    struct TBWGConHeader header;
    uint32_t eventerCount;
};

// pkgcode: 37
struct TBWGConEventerInformation {
    unsigned int eventerCode;
    id_number ID;
    uint32_t energy, spellEnergy;
    digits32 eventer_type, required_informations;
    char name[32];
    struct EventerUses costs;
};

// pkgcode: 38
struct TBWGConEventerOptionsInformation {
    struct TBWGConHeader header;
    id_number chooserId;
    digits32 allowedEventerTypes;
    struct EventerUses restUses;
    struct TBWGConEventerInformation* eventers; // UBERSTRUCTESH!
};


// pkgcode: 39
struct TBWGConTurnPlay {
    struct TBWGConHeader header;
    unsigned int eventer_th;
    struct EventerRequiredInformations requiredInformations;
    unsigned int specs;
};