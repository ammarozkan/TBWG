#include <TBWG_net/tbwgcon1.h>
#include <stdio.h>

#define FASTEFFECTLOOP(i) for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) 

void tbwgcon1ControllerObserve(struct ControllerInterface* intfc, struct ObservingInformation inf)
{
    struct tbwgcon1ControllerInterface* interface = (struct tbwgcon1ControllerInterface*)intfc;
    printf("%s receives a controller observe!\n",interface->name);

    struct TBWGConObservingInformationHeader head;
    head.eventerCount = inf.eventerCount;
    head.characterInformationCount = inf.characterCount;
    head.entityInformationCount = 0; //inf.entityCount normally but entities are notdtettt
    for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) head.effectCounts[i] = getElementCountOfList(inf.effects[i]);
    tbwgcon1SendPackage(interface->cl_sck, &head, TBWGCON1_OBSERVINGINFORMATIONHEADER, sizeof(head));
    
    
    // lets get ready the datas dawg


    unsigned int pkgsize = 0;
    FASTEFFECTLOOP(i) {
        pkgsize += sizeof(struct TBWGConEffectInformation)*head.effectCounts[i];
    }
    pkgsize += sizeof(struct TBWGConUsersEventerInformation)*head.eventerCount;
    pkgsize += sizeof(struct CharacterInformation)*head.characterInformationCount;
    pkgsize += sizeof(struct EntityInformation)*head.entityInformationCount;
    pkgsize += sizeof(struct TBWGConObservingInformation);

    struct TBWGConObservingInformation* realpkg = tbwgmalloc(pkgsize);
    void* megaptrofpkg = (void*) realpkg;

    void* startofuberstructesh = megaptrofpkg+sizeof(struct TBWGConObservingInformation);
    struct TBWGConEffectInformation* effects[EFFECT_TRIGGER_TYPE_COUNT]; // UBERSTRUCTESH
    effects[0] = startofuberstructesh;
    for(unsigned int i = 1 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1) {
        effects[i] = effects[i-1]+sizeof(struct TBWGConEffectInformation)*head.effectCounts[i-1];
    }
    struct TBWGConUsersEventerInformation* eventers = (void*)(effects[EFFECT_TRIGGER_TYPE_COUNT-1]+head.effectCounts[EFFECT_TRIGGER_TYPE_COUNT-1]); // UBERSTRUCTESH!
    struct CharacterInformation* charInfos = (void*)(eventers+head.eventerCount); // UBERSTRUCTESH!
    struct EntityInformation* entityInfos = (void*)(charInfos+head.characterInformationCount); // UBERSTRUCTESH!

    void* obsSame = &(realpkg->selfid);
    tbwgmemcpy(obsSame, &inf,sizeof(struct TBWGConObservingInformation));

    FASTEFFECTLOOP(i) {
        unsigned int j = 0 ;
        ITERATE(inf.effects[i], effectElm_pure) {
            struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;

            effects[i][j].ID = effect->ID;
            effects[i][j].code = effect->code;
            j+=1;
        }
    }

    for(unsigned int i = 0 ; i < head.eventerCount ; i += 1) {
        eventers[i].eventerCode = inf.eventers[i]->eventerCode;
        eventers[i].ID = inf.eventers[i]->ID;
        eventers[i].energyValueType = inf.eventers[i]->energyValueType;
        eventers[i].energy = inf.eventers[i]->baseEnergy;
        eventers[i].spellEnergy = inf.eventers[i]->baseSpellEnergy;
        eventers[i].eventer_type = inf.eventers[i]->eventer_type;
        eventers[i].required_informations = inf.eventers[i]->required_informations;
        tbwgmemcpy(eventers[i].name, inf.eventers[i]->name, 32);
        eventers[i].costs = inf.eventers[i]->costs;
    }

    for(unsigned int i = 0 ; i < head.characterInformationCount ; i += 1) {
        charInfos[i] = inf.charInfos[i];
    }

    for(unsigned int i = 0 ; i < head.entityInformationCount ; i += 1) {
        entityInfos[i] = inf.entityInfos[i];
    }

    tbwgcon1SendPackage(interface->cl_sck, realpkg, TBWGCON1_OBSERVINGINFORMATION, pkgsize);
    free(realpkg);
}

void tbwgcon1ReceiveWorldEvent(struct ControllerInterface* intfc, struct WorldEventInformation inf)
{
    
    struct tbwgcon1ControllerInterface* interface = (struct tbwgcon1ControllerInterface*)intfc;
    printf("%s receives a world event!\n",interface->name);
    
    return;
    
    struct TBWGConWorldEventInformation pkg;
    pkg.selfid = inf.selfid;
    tbwgmemcpy(pkg.eventName,inf.eventName,tbwgstrlen(inf.eventName)+1);
    pkg.position = inf.position;
    printf("WORLDEVENTSENDING\n");
    tbwgcon1SendPackage(interface->cl_sck,&pkg,TBWGCON1_WORLDEVENTINFORMATION, sizeof(pkg));
}


/*
struct TBWGConEventerInformation {
    unsigned int eventerCode;
    id_number ID;
    uint32_t energy, spellEnergy;
    digits32 eventer_type, required_informations;
    char name[32];
    struct EventerUses costs;
};
*/

struct TurnPlay tbwgcon1ControllerChooseEventer(struct ControllerInterface* intfc, id_number chooserId, 
    digits32 allowedEventerTypes, size_t eventerCount, struct Eventer** eventers, struct EventerUses restUses)
{
    struct TurnPlay turnPlay;

    struct tbwgcon1ControllerInterface* interface = (struct tbwgcon1ControllerInterface*)intfc;
    printf("%s receives a ChooseEventer request!\n",interface->name);


    struct TBWGConEventerOptionsInformationHeader head;
    head.chooserId = chooserId;
    head.allowedEventerTypes = allowedEventerTypes;
    head.restUses = restUses;
    head.eventerCount = eventerCount;
    tbwgcon1SendPackage(interface->cl_sck, &head, TBWGCON1_EVENTEROPTIONSINFORMATIONHEADER, sizeof(head));

    unsigned int eventerOptionInformationPtrSize = sizeof(struct TBWGConEventerOptionsInformation);
    eventerOptionInformationPtrSize += sizeof(struct TBWGConEventerInformation)*eventerCount;
    void* eventerOptionInformationPtr = malloc(eventerOptionInformationPtrSize);
    struct TBWGConEventerInformation* informations = eventerOptionInformationPtr + sizeof(struct TBWGConEventerOptionsInformation);

    for(unsigned int i = 0 ; i < eventerCount ; i += 1) {
        informations[i] = tbwgconConvertToEventerInformation(*(eventers[i]));
    }
    tbwgcon1SendPackage(interface->cl_sck, eventerOptionInformationPtr, TBWGCON1_EVENTEROPTIONSINFORMATION, eventerOptionInformationPtrSize);
    free(eventerOptionInformationPtr);


    int recvment = tbwgcon1ReceivePackage(interface->cl_sck, GLB_RECV, TBWGCON1_TURNPLAY);
    int r;
    if (r < 0) {
        DEBUG_PRINT("tbwgcon1ControllerChooseEventer","Theres no package to read for TBWGCON1_TURNPLAY package.");
        return turnPlay;
    } else if(r == 0) {
        DEBUG_PRINT("tbwgcon1ControllerChooseEventer","Receiver wanted to read TBWGCON1_TURNPLAY package but another came along.");
        DEBUG_PRINTUINT("tbwgcon1ControllerChooseEventer","Heres the came package.",GLB_RECV_HEAD->pkgcode);
        return turnPlay;
    }

    struct TBWGConTurnPlay* tp_pkg = (struct TBWGConTurnPlay*)GLB_RECV;

    turnPlay.eventer_th = tp_pkg->eventer_th;
    turnPlay.requiredInformations = tp_pkg->requiredInformations;
    turnPlay.specs = tp_pkg->specs;

    return turnPlay;
}


struct ControllerInterface* tbwgcon1GetNetworkedControllerInterface(int socket,char* name)
{
    struct tbwgcon1ControllerInterface* result;
    result = tbwgmalloc(sizeof(*result));
    result->interface.observer = tbwgcon1ControllerObserve;
    result->interface.receiveWorldEvent = tbwgcon1ReceiveWorldEvent;
    result->interface.chooseEventer = tbwgcon1ControllerChooseEventer;
    result->cl_sck = socket;
    result->name = name;
    return (struct ControllerInterface*)result;
}