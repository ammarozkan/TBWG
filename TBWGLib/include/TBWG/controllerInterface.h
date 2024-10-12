#ifndef TBWG_CONTROLLERINTERFACE_H
#define TBWG_CONTROLLERINTERFACE_H

#include <TBWG/observation.h>
#include <TBWG/eventer.h>

struct ControllerInterface;

struct EventerInformation {
    unsigned int eventerCode;
    char name[32];

    uint8_t energyValueType;
    int minEnergy, minSpellEnergy;
    int maxEnergy, maxSpellEnergy;

    struct EventerUses costs;
};

struct EventerInformation* getEventerInformationsFromEventers(size_t eventerCount, struct Eventer* eventers);

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation);
// and all the other eventer etc. things here please

typedef void (*ReceiveWorldEvent)(struct ControllerInterface*, struct WorldEventInformation);

typedef struct TurnPlay (*ControllerChooseEventer)(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers, struct EventerUses restUses);

struct ControllerInterface {
    // receivers
    ControllerObserve observer;
    ReceiveWorldEvent receiveWorldEvent;

    // request responsers
    ControllerChooseEventer chooseEventer;
};

struct ControllerInterface* getDefaultControllerInterface();

struct ControllerInterface* getstdioControllerInterface();

#endif /*TBWG_CONTROLLERINTERFACE_H*/