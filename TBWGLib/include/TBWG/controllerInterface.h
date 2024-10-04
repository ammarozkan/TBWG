#ifndef TBWG_CONTROLLERINTERFACE_H
#define TBWG_CONTROLLERINTERFACE_H

#include <TBWG/ccs.h>

struct ControllerInterface;

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation*);
// and all the other eventer etc. things here please

struct ControllerInterface {
    struct Controller* controller;
    ControllerObserve observer;
};

void sendObservationToController(struct ControllerInterface*, struct ObservingInformation obsInfo);

struct ControllerInterface getDefaultControllerInterface(struct Controller* controller);

#endif /*TBWG_CONTROLLERINTERFACE_H*/