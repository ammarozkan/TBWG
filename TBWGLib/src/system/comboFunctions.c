#include <TBWG/system/comboFunctions.h>
#include <TBWG/attacks.h>
#include <TBWG/essentials.h>

struct ComboFunction tbwgGetComboFunction(ComboFunctionExecuterSkeleton executer)
{
    struct ComboFunction comboFunction = {.functions = createList()};

    comboFunction.executer = executer;
    
    return comboFunction;
}


struct ComboFunction tbwgGetComboFunction2(ComboFunctionExecuterSkeleton executer,SoloFunctionSkeleton f, void*ptr)
{
    struct ComboFunction comboFunction = tbwgGetComboFunction(executer);
    tbwgComboFunctionAddSoloFunction(&comboFunction, f, ptr);
    return comboFunction;
}

id_number tbwgComboFunctionAddSoloFunction(struct ComboFunction* c,SoloFunctionSkeleton f, void*ptr)
{
    struct SoloFunction s = {.function = f, .ptr = ptr, .ID = getID()};

    return tbwgComboFunctionAddSoloFunctionStruct(c, s);
}


id_number tbwgComboFunctionAddSoloFunctionStruct(struct ComboFunction* c,struct SoloFunction s)
{
    struct ComboFunctionElementHeader cfeh;
    cfeh.soloFunction = s;
    prependElement(&(c->functions), &cfeh, sizeof(cfeh));
    return s.ID;
}

int tbwgComboFunctionRemoveSoloFunction(struct ComboFunction* c, id_number ID)
{
    int removedAmount = 0;
    ITERATE(c->functions, _function) {
        struct SoloFunction soloFunction = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        if (soloFunction.ID == ID) {
            ITERATION_DESTROY(c->functions, _function);
            removedAmount += 1;
        }
    }
    return removedAmount;
}

/*

StreamType tbwgComboFunctionExecuter(struct ComboFunction* c, StreamType a)
{
    struct ComboPositionlessInstructors instructors = {.importanceLevel = -1};

    int currentImportance = -1;

    ITERATE(c->functions, _function) {
        struct SoloFunction sf = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        SoloFunctionSkeleton function = sf.function;

        StreamType b = ((tbwgComboFunctionSoloFunction)function)(sf.ptr,&instructors,  a);
        if(instructors.importanceLevel == -1 || instructors.importanceLevel > currentImportance) {
            a = b;
            currentImportance = instructors.importanceLevel;
            instructors.importanceLevel = -1;
        }
    }
    return a;
}

*/


// testing combo function


typedef int (*tbwgComboFunctionSoloFunction_i)(void*ptr,struct ComboPositionlessInstructors*, int);

int tbwgComboFunctionExecuter_i(struct ComboFunction* c, int stream)
{
    struct ComboPositionlessInstructors instructors = {.importanceLevel = -1, .currentImportance = -1};

    ITERATE(c->functions, _function) {
        struct SoloFunction sf = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        SoloFunctionSkeleton function = sf.function;

        int _stream = ((tbwgComboFunctionSoloFunction_i)function)(sf.ptr,&instructors,  stream);
        if(instructors.currentImportance == -1 || instructors.importanceLevel > instructors.currentImportance) {
            stream = _stream;
            instructors.currentImportance = instructors.importanceLevel;
            instructors.importanceLevel = -1;
        }
    }
    return stream;
}


typedef int (*tbwgComboFunctionSoloFunction_pi)(void*ptr,struct ComboPositionlessInstructors*, void*,int);

int tbwgComboFunctionExecuter_pi(struct ComboFunction* c, void* p, int stream)
{
    struct ComboPositionlessInstructors instructors = {.importanceLevel = -1, .currentImportance = -1};

    ITERATE(c->functions, _function) {
        struct SoloFunction sf = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        SoloFunctionSkeleton function = sf.function;

        int _stream = ((tbwgComboFunctionSoloFunction_pi)function)(sf.ptr,&instructors,  p,stream);
        if(instructors.currentImportance == -1 || instructors.importanceLevel > instructors.currentImportance) {
            stream = _stream;
            instructors.currentImportance = instructors.importanceLevel;
            instructors.importanceLevel = -1;
        }
    }
    return stream;
}

typedef struct AttackInfo (*tbwgComboFunctionSoloFunction_ppA)(void*ptr,struct ComboPositionlessInstructors*, void*,void*,struct AttackInfo);

struct AttackInfo tbwgComboFunctionExecuter_ppA(struct ComboFunction* c, void* p1, void* p2, struct AttackInfo stream)
{
    struct ComboPositionlessInstructors instructors = {.importanceLevel = -1, .currentImportance = -1};

    ITERATE(c->functions, _function) {
        struct SoloFunction sf = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        SoloFunctionSkeleton function = sf.function;

        struct AttackInfo _stream = ((tbwgComboFunctionSoloFunction_ppA)function)(sf.ptr,&instructors,  p1,p2,stream);
        if(instructors.currentImportance == -1 || instructors.importanceLevel > instructors.currentImportance) {
            stream = _stream;
            instructors.currentImportance = instructors.importanceLevel;
            instructors.importanceLevel = -1;
        }
    }
    return stream;
}

typedef void* (*tbwgComboFunctionSoloFunction_pp)(void*ptr,struct ComboPositionlessInstructors*, void*,void*);

void* tbwgComboFunctionExecuter_pp(struct ComboFunction* c, void* p, void* stream)
{
    struct ComboPositionlessInstructors instructors = {.importanceLevel = -1, .currentImportance = -1};

    ITERATE(c->functions, _function) {
        struct SoloFunction sf = ((struct ComboFunctionElementHeader*)_function)->soloFunction;
        SoloFunctionSkeleton function = sf.function;

        void* _stream = ((tbwgComboFunctionSoloFunction_pp)function)(sf.ptr,&instructors,  p,stream);
        if(instructors.currentImportance == -1 || instructors.importanceLevel > instructors.currentImportance) {
            stream = _stream;
            instructors.currentImportance = instructors.importanceLevel;
            instructors.importanceLevel = -1;
        }
    }
    return stream;
}