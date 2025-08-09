// Combo functions are a solution to the requirement of pre functions
// to some functions. Those combo functions should be quickly subtractable,
// executable comboingly, and distinguishable as which is added by something etc.
//
// DEFINITION
//
// a SoloFuncion has pre-parameter and a stream parameter at the end.
// that stream parameter, streams across all the SoloFunctions in a
// Combo function.
//
// A solo function shall follow this:
// streamParameter SoloFunction(void*ptr, struct ComboPositionlessInstructors*, parameter1, parameter2, ... , parametern, streamParameter);
// 
// typless ones can be in any type. first pointer shall be the void* for persistent stuff.

#ifndef TBWG_COMBOFUNCTIONS_H
#define TBWG_COMBOFUNCTIONS_H

#include <TBWG/essentials.h>
#include <TBWG/lists.h>

struct ComboPositionlessInstructors;

typedef void (*SoloFunctionSkeleton)(void*ptr,struct ComboPositionlessInstructors*instructors);
typedef void (*ComboFunctionExecuterSkeleton)();

typedef SoloFunctionSkeleton ComboSolo;
typedef ComboFunctionExecuterSkeleton ComboExec;

struct SoloFunction {
    id_number ID;
    SoloFunctionSkeleton function;
    void* ptr; // for transacting a ref. for presistent stuff
};

struct ComboFunctionElementHeader {
	struct ListElementHeader header;
	struct SoloFunction soloFunction;
};

struct ComboPositionlessInstructors {
    int importanceLevel; // if its changed, the most importance leveled stuff will get the returning chance.
    int currentImportance; // current importance requirement for variable change
};

SoloFunctionSkeleton tbwgGetSoloFunctionFromElementHeader(struct ComboFunctionElementHeader*);

struct ComboFunction {
    struct List functions;
    ComboFunctionExecuterSkeleton executer;
};


// creates combo function that has the executer
struct ComboFunction tbwgGetComboFunction(ComboFunctionExecuterSkeleton executer);

// creates the combo function and adds the first solo function
struct ComboFunction tbwgGetComboFunction2(ComboFunctionExecuterSkeleton executer,SoloFunctionSkeleton f, void*ptr);

// adds a solo function to the start
id_number tbwgComboFunctionAddSoloFunction(struct ComboFunction*,SoloFunctionSkeleton,void*ptr); // returns to attached id for that function

// adds a solo function to the start
id_number tbwgComboFunctionAddSoloFunctionStruct(struct ComboFunction* c,struct SoloFunction s); // returns to attached id for that function

// removes all the solo functions that has the id of ID
int tbwgComboFunctionRemoveSoloFunction(struct ComboFunction*, id_number ID);

// reusable combo function executers tbwgComboFunctionExecuter_aaa...aaab
// a is the parameter types
// b is the streaming parameter type

typedef int (*tbwgComboFunctionExecuter_iType)(struct ComboFunction*, int);
int tbwgComboFunctionExecuter_i(struct ComboFunction*, int);

typedef int (*tbwgComboFunctionExecuter_piType)(struct ComboFunction*, void*, int);
int tbwgComboFunctionExecuter_pi(struct ComboFunction*, void*, int);

typedef struct AttackInfo (*tbwgComboFunctionExecuter_ppAType)(struct ComboFunction*, void*, void*, struct AttackInfo);
struct AttackInfo tbwgComboFunctionExecuter_ppA(struct ComboFunction*, void*, void*, struct AttackInfo);

typedef struct AttackInfo (*tbwgComboFunctionExecuter_ppType)(struct ComboFunction*, void*, void*);
void* tbwgComboFunctionExecuter_pp(struct ComboFunction* c, void* p1, void* p2);


#endif /*TBWG_COMBOFUNCTIONS_H*/