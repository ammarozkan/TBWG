#include <TBWG/CCS.h>

struct ControllerInterface;

typedef void (*ControllerObserve)(struct Character* character, struct ObservingInformation* worldInfo);
// and all the other eventer etc. things here please

struct ControllerInterface {
    struct Controller* controller;
    ControllerObserve observer;
};

void sendObservationToController(struct ControllerInterface*, struct ObservingInformation worldInfo);

struct ControllerInterface getDefaultControllerInterface(struct Controller* controller);