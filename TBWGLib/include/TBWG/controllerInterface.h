#ifndef TBWG_CONTROLLERINTERFACE_H
#define TBWG_CONTROLLERINTERFACE_H

#include <TBWG/observation.h>

struct ControllerInterface;

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation);
// and all the other eventer etc. things here please

typedef struct EventerChoose (*ControllerChooseEventer)(struct ControllerInterface*, digits32 allowedEventerTypes, 
    size_t eventerCount, struct Eventer* eventers);

struct ControllerInterface {
    ControllerObserve observer;
    ControllerChooseEventer chooseEventer;
};

struct ControllerInterface* getDefaultControllerInterface();

struct ControllerInterface* getstdioControllerInterface();

#endif /*TBWG_CONTROLLERINTERFACE_H*/