#ifndef TBWG_CONTROLLERINTERFACE_H
#define TBWG_CONTROLLERINTERFACE_H

#include <TBWG/observation.h>

struct ControllerInterface;

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation);
// and all the other eventer etc. things here please

typedef void (*ReceiveWorldEvent)(struct ControllerInterface*, struct WorldEventInformation);

typedef struct TurnPlay (*ControllerChooseEventer)(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers);

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