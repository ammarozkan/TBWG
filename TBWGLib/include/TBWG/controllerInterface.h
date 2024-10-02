#include <TBWG/CCS.h>

struct ControllerInterface;

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation*);
// and all the other eventer etc. things here please

struct ControllerInterface {
    struct Controller* controller;
    ControllerObserve observer;
};

void sendObservationToController(struct ControllerInterface*, struct ObservingInformation obsInfo);

struct ControllerInterface getDefaultControllerInterface(struct Controller* controller);