#include <TBWG/lists.h>

struct Controller {
    int socket_fd;
    char* nickname;
};

struct ControllerCommunicatingSystem {
    struct List controllers;
} CCS;




enum RequestType = {REQUEST_NONE, REQUEST_OBSERVE, REQUEST_SENDMESSAGE, REQUEST_EVENTER};
enum ResponseType = {RESPONSE_MESSAGE, RESPONSE_OBSERVE, RESPONSE_EVENTER};

struct RequestHeader {
    enum RequestType type;
};


struct ObserveRequest{
    struct RequestHeader header = {.type = REQUEST_OBSERVE};
};

struct SendMessageRequest{
    struct RequestHeader header = {.type = REQUEST_SENDMESSAGE};

    size_t messageSize;
    char* message;

    unsigned int aswho; // 0 if nickname, else that id numbered character
};

struct EventerRequest{
};


struct ResponseHeader{
    enum ResponseType type;
};

struct MessageResponse {
    struct ResponseHeader header = {.type = RESPONSE_MESSAGE};

    size_t messageSize;
    char* message;

    unsigned int aswho; // 0 if nickname, else that id numbered character
};

struct ObserveResponse {
    struct ResponseHeader header = {.type = RESPONSE_OBSERVE};

    struct ObservingInformation observingInformation;
};

struct EventerResponse{};


struct RequestHeader* receiveRequest(struct Controller controller);
struct ResponseHeader* receiveResponse(struct Controller controller);
void sendResponse(struct Controller controller, struct ResponseHeader* response);
void sendRequest(struct Controller controller, struct RequestHeader* request);