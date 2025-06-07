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
    unsigned int effectsCount = 0;
    FASTEFFECTLOOP(i) {
        pkgsize += sizeof(struct TBWGConObservingEffectInformation)*head.effectCounts[i];
        effectsCount += head.effectCounts[i];
    }
    pkgsize += sizeof(struct TBWGConObservingEventerInformation)*head.eventerCount;
    pkgsize += sizeof(struct TBWGConObservingCharacterInformation)*head.characterInformationCount;
    pkgsize += sizeof(struct TBWGConObservingEntityInformation)*head.entityInformationCount;
    pkgsize += sizeof(struct TBWGConObservingInformation);

    struct TBWGConObservingInformation* realpkg = tbwgmalloc(pkgsize);
    void* megaptrofpkg = (void*) realpkg;

    void* startofuberstructesh = megaptrofpkg+sizeof(struct TBWGConObservingInformation);
    struct TBWGConObservingEffectInformation* effects = startofuberstructesh; // UBERSTRUCTESH
    struct TBWGConObservingEventerInformation* eventers = (void*)(effects+effectsCount); // UBERSTRUCTESH!
    struct TBWGConObservingCharacterInformation* charInfos = (void*)(eventers+head.eventerCount); // UBERSTRUCTESH!
    struct TBWGConObservingEntityInformation* entityInfos = (void*)(charInfos+head.characterInformationCount); // UBERSTRUCTESH!

    void* obsSame = &(realpkg->selfid);
    tbwgmemcpy(obsSame, &inf,sizeof(struct TBWGConObservingInformation));
    printf("ROT:%f,%f\n",realpkg->direction.x, realpkg->direction.y);


    unsigned int j = 0;

    FASTEFFECTLOOP(i) {
        ITERATE(inf.effects[i], effectElm_pure) {
            struct Effect* effect = ((struct EffectListElement*)effectElm_pure)->effect;

            effects[j].ID = effect->ID;
            effects[j].code = effect->code;
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
        {
            eventers[i].costs.classic = inf.eventers[i]->costs.classic;
            eventers[i].costs.fastcombat = inf.eventers[i]->costs.fastcombat;
            eventers[i].costs.movement = inf.eventers[i]->costs.movement;
            eventers[i].costs.fastmagic = inf.eventers[i]->costs.fastmagic;
            eventers[i].costs.thoughtmagic = inf.eventers[i]->costs.thoughtmagic;
        }
    }

    for(unsigned int i = 0 ; i < head.characterInformationCount ; i += 1) {
        charInfos[i].ID = inf.charInfos[i].ID;
        charInfos[i].characterCode = inf.charInfos[i].characterCode;
        charInfos[i].position = inf.charInfos[i].position;
        charInfos[i].direction = inf.charInfos[i].direction;
        charInfos[i].hp = inf.charInfos[i].hp;
    }

    for(unsigned int i = 0 ; i < head.entityInformationCount ; i += 1) {
        entityInfos[i].ID = inf.entityInfos[i].ID;
        entityInfos[i].entityCode = inf.entityInfos[i].entityCode;
        entityInfos[i].position = inf.entityInfos[i].position;
        entityInfos[i].direction = inf.entityInfos[i].direction;
    }

    tbwgcon1SendPackage(interface->cl_sck, realpkg, TBWGCON1_OBSERVINGINFORMATION, pkgsize);
    free(realpkg);
}

void tbwgcon1ReceiveWorldEvent(struct ControllerInterface* intfc, struct WorldEventInformation inf)
{
    
    struct tbwgcon1ControllerInterface* interface = (struct tbwgcon1ControllerInterface*)intfc;
    printf("%s receives a world event!\n",interface->name);
    
    struct TBWGConWorldEventInformation pkg;
    pkg.selfid = inf.selfid;
    tbwgmemcpy(pkg.eventName,inf.eventName,tbwgstrlen(inf.eventName)+1);
    pkg.position = inf.position;
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
    {
        head.restUses.classic = restUses.classic;
        head.restUses.fastcombat = restUses.fastcombat;
        head.restUses.movement = restUses.movement;
        head.restUses.fastmagic = restUses.fastmagic;
        head.restUses.thoughtmagic = restUses.thoughtmagic;
    }
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
    {
        turnPlay.requiredInformations.position = tp_pkg->requiredInformations.position;
        turnPlay.requiredInformations.position2 = tp_pkg->requiredInformations.position2;
        turnPlay.requiredInformations.direction = tp_pkg->requiredInformations.direction;
        turnPlay.requiredInformations.A = tp_pkg->requiredInformations.A;
        turnPlay.requiredInformations.B = tp_pkg->requiredInformations.B;
    }
    turnPlay.specs = tp_pkg->specs;

    DEBUG_PRINTUINT("tbwgcon1ControllerChooseEventer","EVENTER_TH",tp_pkg->eventer_th);
    DEBUG_PRINTUINT("tbwgcon1ControllerChooseEventer","SPECS",tp_pkg->specs);
    DEBUG_PRINTUINT("tbwgcon1ControllerChooseEventer","REQPOSx",tp_pkg->requiredInformations.position.x);

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