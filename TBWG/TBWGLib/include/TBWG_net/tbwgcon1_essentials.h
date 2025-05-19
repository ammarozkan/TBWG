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

#define TBWGCON1_STD_NAME_SIZE 32
#define TBWGCON1_STD_EVENTNAME_SIZE TBWGCON1_STD_NAME_SIZE


struct TBWGConCharacterInformation {
    uint32_t code;
    iValue hp, e, se;
};


struct TBWGConPtsizedCharacterInformation {
    void* systematicPtr; // a pointer to help the local tbwgmanager find the character. will be returned back to the system
    struct TBWGConCharacterInformation inf;
};

struct TBWGConPtsizedCharacterInformationListElement {
    struct ListElementHeader header;
    struct TBWGConPtsizedCharacterInformation charinf;
};

struct TBWGConServerResult {
    int socket;
    char* name;
    struct TBWGConPtsizedCharacterInformation midinf;
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



#define TBWGCON1_ENTERINGPACKAGE 0
// pkgcode : 0
struct TBWGConEnteringPackage {
    struct TBWGConHeader header; // tbwgname and version will be checked there
    uint32_t namesize;
    char name[TBWGCON1_STD_NAME_SIZE]; // max 32 sized
};

#define TBWGCON1_ENTERINGRESPONSE 1
// pkgcode : 1
struct TBWGConEnteringResponse {
    struct TBWGConHeader header; // returns the closest tbwgname and version
    uint32_t errcode;
    uint32_t nextchapter; // returns 1 if version not supported else 2
};

// pkgcode : 9
struct TBWGConQuit {
    struct TBWGConHeader header;
    uint32_t errcode;
};

#define TBWGCON1_MAX_AVAILABLE_CHARACTER_COUNT 16

#define TBWGCON1_CHARACTERINFORMATOR 4
// pkgcode : 4
struct TBWGConCharacterInformator {
    struct TBWGConHeader header;

    uint16_t characterCount;
    struct TBWGConCharacterInformation charinfo[TBWGCON1_MAX_AVAILABLE_CHARACTER_COUNT];
};

#define TBWGCON1_CHARACTERSELECTION 5
// pkgcode : 5
struct TBWGConCharacterSelection {
    struct TBWGConHeader header;

    uint8_t selection;
    uint8_t options;
};


#define TBWGCON1_CHARSELECTERR_NOPERMISSION 0x0
#define TBWGCON1_CHARSELECTERR_ALREADYINUSE 0x1
#define TBWGCON1_CHARSELECTERR_CHOOSEDABOVE 0x2

#define TBWGCON1_CHARACTERSELECTIONERROR 6
// pkgcode : 6
struct TBWGConCharacterSelectionError {
    struct TBWGConHeader header;

    uint32_t errcode;
    uint32_t nextchapter;
};



//// ////  Playing  //// ////

// pkgcode: 33
#define TBWGCON1_OBSERVINGINFORMATIONHEADER 33
struct TBWGConObservingInformationHeader {
    struct TBWGConHeader header;
    uint32_t effectCounts[EFFECT_TRIGGER_TYPE_COUNT];
    uint32_t eventerCount;
    uint32_t characterInformationCount;
    uint32_t entityInformationCount;
};


#define TBWGCON1_OBSERVINGINFORMATION 34
// pkgcode: 34
struct TBWGConObservingInformation {
    struct TBWGConHeader header;
    id_number selfid;

    struct Stats characterStats;
    iValue hp, e, se;
    iVector position; fVector direction;
    digits32 state;

    //struct TBWGConEffectInformation* effects[EFFECT_TRIGGER_TYPE_COUNT]; // UBERSTRUCTESH
    //struct TBWGConUsersEventerInformation* eventers; // UBERSTRUCTESH!
    //struct CharacterInformation* charInfos; // UBERSTRUCTESH!
    //struct EntityInformation* entityInfos; // UBERSTRUCTESH!
};

#define TBWGCON1_WORLDEVENTINFORMATION 35
// pkgcode: 35
struct TBWGConWorldEventInformation {
    struct TBWGConHeader header;
    id_number selfid;
    char eventName[TBWGCON1_STD_EVENTNAME_SIZE];
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