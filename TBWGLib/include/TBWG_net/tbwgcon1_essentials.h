//	tbwgcon1_essentials.h is the file that contains structs that defined in TBWGCON1


#define TBWGCON1_ERR_UNSUPPORTEDVERSION (1<<0)
#define TBWGCON1_ERR_UNSUPPORTEDGAME (1<<1)
#define TBWGCON1_ERR_UNEXPECTEDPKG (1<<2)

#define TBWGCON1_CHRSLCTOPT_CREATE (1 << 0)

#define TBWGCON1_CHRSLCTERR_NOPERMISSION 0x0
#define TBWGCON1_CHRSLCTERR_ALREADYINUSE 0x1

#define STD_NAME_SIZE 32
#define STD_EVENTNAME_SIZE STD_NAME_SIZE


struct TBWGConHeader {
	char[4] tbwgname;
	uint8_t version[3];
	uint8_t pkgcode;
};

// pkgcode : 7
struct TBWGWait {
	struct TBWGConHeader header;
};

// pkgcode : 0
struct TBWGCheckingPackage {
	char[4] tbwgname;
	uint8_t version[3];
};

// pkgcode : 1
struct TBWGWelcomingPackage {
	struct TBWGCheckingPackage ip;
	uint32_t errcode;
	uint32_t nextchapter;
};

// pkgcode : 2
struct TBWGIntroducementPackage {
	struct TBWGConHeader header;
	uint32_t nameSize;
	char name[STD_NAME_SIZE];
};

// pkgcode : 3
struct TBWGIntroducementResponse {
	struct TBWGConHeader header;
	uint32_t errcode;
	uint32_t nextchapter;
};

// pkgcode : 4
struct TBWGCharacterInformator {
	struct TBWGConHeader header;

	uint16_t characterCount;
	struct TBWGCONCharacterInfo charinfo[characterCount];
};

// pkgcode : 5
struct TBWGCharacterSelection {
	struct TBWGConHeader header;

	uint8_t selection;
	uint8_t options; // CHARACTERSELECTION_OPTION_CREATE, etc.
};

// pkgcode : 6
struct TBWGCharacterSelectionError {
	struct TBWGConHeader header;

	uint32_t characterSelectionErrorCode;
	uint32_t nextchapter;
};

// pkgcode : 8
struct TBWGNewCharacterInfo {
	struct TBWGConHeader header;
	struct TBWGCONCharacterInfo charinfo;
};



// pkgcode: 200 (y)
struct TBWGSure {
    struct TBWGConHeader header;
    char name[STD_NAME_SIZE];
    struct TBWGCONCharacterInfo charinfo;
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

    struct List effects[EFFECT_TRIGGER_TYPE_COUNT];
    struct Eventer eventers[eventerCount];
    struct CharacterInformation charInfos[CharacterInformationCount];
    struct EntityInformation entityInfos[entityInformationCount];
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
    struct EventerInformation eventers[eventerCount]; // a static struct that has values of informations for a eventer
};


// pkgcode: 39
struct TBWGConTurnPlay {
    struct TBWGConHeader header;
    unsigned int eventer_th;
    struct EventerRequiredInformations requiredInformations;
    unsigned int specs;
};