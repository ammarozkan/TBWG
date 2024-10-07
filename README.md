### Explanation

This game rule standard is created to battle with a lot of creative powers.

# TBWG 1

# Game Logic


## Queues
First base logic of this game is the game queue.


Activities on queues:
#### Character Turns
When a turn of Character came up, that Character should make
the move.

#### Reorder Turns
When this event came up on the queue, the base queue should be
added to the queue.

Base queue is the queue derieven from Character's base queues. In example
when Character 1 is the first and Character 2 is the second, and their base
queues are:

Character 1's Base Queue:

- Character 1 Normal Eventer Turn (Every Type of Events)
- Character 1 Karate Advantage Attack Turn (Fight Attacks)

and Character 2's Base Queue:

- Character 2 Normal Attack Turn

The base queue will be (if Character 1 is the first attacker)

- Character 1 Normal Attack Turn
- Character 1 Karate Advantage Attack Turn
- Character 2 Normal Attack Turn

When this happens, that means a round is ended and new
round has been passed to. And that means 3 seconds passed.

#### Timed Effect
When a timed effect came up on the queue, this effect will
be executed. In example a timed bomb.

#### Entity Turns
When an object is throwen or something falling from sky, this entity
will take a place in the base queue. In example a rock is flying
on the sky. That rock's base queue will be:

- Rock's Turn (Physical Fall)

When this turn executed, rock could move and hurt who is on the place.
And then disappear (destroy itself).


#### Example fight for queues

- Character 1's Turn
- Character 2's Turn


Character 1, created a fire breath to Character 2. (no change on queue)

Queue:

- Character 2's Turn

Character 2 stopped the time. (that will add approximately 3 Character 2 Turns)

Queue:

- Character 2's Turn
- Character 2's Turn
- Character 2's Turn

Character 2 moves towards to Character 1.

Queue:

- Character 2's Turn
- Character 2's Turn

Character 2 puts a timed bomb to near to Character 1.

Queue:

- Character 2's Turn
- Reorder Turns
- Reorder Turns
- Reorder Turns
- Timed Bomb Explosion (effect)

Character 2 moves backwards.


Queue:

- Reorder Turns
- Reorder Turns
- Reorder Turns
- Timed Bomb Explosion (effect)

Reoder Happens. Adds the base queue.


Queue:

- Character 1's Turn
- Character 2's Turn
- Reorder Turns
- Reorder Turns
- Timed Bomb Explosion (effect)


## Characters

Characters has stats that Strength(STR),
Dexterity(DEX), Constitution(CNS), Wisdom(WIS),
Spell Constitution(SCS)

A damage can be taken from Head, Body, Arms, Legs.
Armors and body parts can resist to damage. In example
lets say Heavy Iron armor can resist to 1d6 dmg.
sharp damages and the body part of this character can resist
to any 1d6 dmg. Let's say we got 4 sharp dmg. Armor resisted to
3 dmg so we have 1 dmg. And the character resisted 1 so we got nothing
left.

Also before hitting, a DEX competition should be made.

1d6 + DEX_1 vs 1d6 + DEX_defencer (+ 4 for head, + 2 for legs)


#### Body Resistors

Body: CNS

Arm: CNS - 2

Legs: CNS - 1

Head: CNS - 4


#### Energy Standartization

Walking 1 Meter : 1 Energy
Strong Punch : 5 Energy

Spending 2 Energy to gain : SCS Spell Energy

Energy Restoration : CNS


#### Spell Energy Standartization

Little wind that moves a paper : 1 Spell Energy
A fire blowing to burn a oak log: 15 Spell Energy

#### Effects
Every character can be effected by some effects. These
effects will be executed in every Reorder Turn.


## Powers

### Passive Powers

Passive powers will change character's base queue, base stats or
add some effects to the character when the fight is starting.

In example karate fighter's speed power can add a "professional attack"
queue to fighter's base queue to make an additional karate attack.


### Active Eventers

These kind of eventers can be used in the turn. Every power has a specific
type. In example punch, karate straight punch, karate lunge punch are
fight powers.



## Example

Let's create Pain (Deva Path) from Naruto as a character.


#### Pain (Deva)

STR: 7
DEX: 10
CNS: 8
WIS: 6
SCS: 16

Passive Powers:

- Uzumaki Blood: Passive 10 Spell Energy regen.

Active Eventers:

- Shinra Tensei [-0 Energy, -20 Spell Energy]: Pushes the target.
- Banshou Tennin [-0 Energy, -14 Spell Energy]: Pulls the target.
- Shinra Tensei (Area) [-0 Energy, -27 Spell Energy]: Pushes all the targets
on an area.
- Chakra Stick Stabbing [-1 Energy, -2 Spell Energy]: 5 + 1d6 sharp damage, 2 rinnepoison
damage.
- Gravity Dash [-1 Energy, -17 Spell Energy]: Can move 4 times faster.
- Chibaku Tensei [-16 Energy, -30 Spell Energy]: 3 parted magic.
    - 1 : Target will effected by "Heavy Gravity" effect. 6 gravity dmg.
    - 2 : 4 gravity dmg on random positions on ground.
    - 3 : Target will be effected by "Chibaku Sealing" effect. 8 gravity dmg.



And DIO from JoJo's Bizzare Adventure


#### DIO

STR: 14
DEX: 11
CNS: 9
WIS: 8
SCS: 0

Passive Powers:

- Vampirism : STR +3, CNS +3
- Time Stopping Movement: Gives the Time Stop Ready effect that gains DEX +6

Active Eventers:

- Eating Humans [-2 Energy, -0 Spell Energy]: Eats a human flesh to regen and gain power.
STR +2 and CNS +2 and Health +5 and Energy +9

- Time Stop [-16 Energy, -0 Spell Energy]: Stops time about 3 rounds if Time Stop Ready
is active. When used, deactivates Time Stop Ready. And puts Time Stop Ready to queue.
About 1 Rounds later.

- Stand Punch [-5 Energy, -0 Spell Energy]: a stand punch. 3d6 bludgeoning dmg.

- Time Stopping Punch [-20 Energy, -0 Spell Energy]: Time stops, then
DIO punches the target. Then time ugokidas. 3 3d6 bludgeoning dmg. Impossible
to dodge except the target has "Time Stop Ready" effect.

Effects:

- Time Stop Ready: DEX +6




# Program System Design

In the program system, there will be two part that seperated from each other: Server
and Controller.

## Deep Down Systems

### Lists

```C
struct ListElementHeader {
    struct ListElementHeader* prior;
    struct ListElementHeader* next;
};

struct List {
    struct ListElementHeader* firstelement;
    struct ListElementHeader* lastelement;
    unsigned int count = 0;
};

#define ITERATE(list, asthis) for(struct ListElementHeader* asthis = list.firstelement; asthis != NULL, asthis = asthis->next)

struct List createList();

void addElement(struct List* list, struct ListElementHeader* element);

void removeElement(struct List* list, struct ListElementHeader* element); // we don't need the list actually.
```

### Identification Numbers

```C
typedef uint32_t id_number;

id_number getID();



id_number getID()
{
    static id_number id = 5;
    return id++;
}
```

## Server

Server creates queue, when a character's turn came up, server will send the information
that character gets and ask the controller to what to do for that character. Executes
effects when the Reorder Turn came up.

Every hit, damage, position change, should be notified to the controllers by eye of the
character. For that, every succesfull hit should be notified. And every object in the world
should have an ID.

A communcation standard should be defined.

These structs should be defined:

### The Queue

```C
enum QueueElementType {
    QUEUE_CHARACTER, QUEUE_ENTITY, QUEUE_TIMED, QUEUE_REORDER
};

struct Queue{
    struct List queueElements;
};

struct QueueElementHeader{
    struct ListElementHeader listHeader;

    enum QueueElementType type;
};
```

#### Fight Start

Every character's every passive will be executed. Passive functions should be
accesed(rw) to characters and character's values.

When a passive adds a stat to the character, will add it to the baseStats in
character struct (take a look at the struct Character).

#### Reorder Turns

Server will add every character's base queue to the start of the queue in order
of character's speed.

```C
struct QueueReorderTurn {
    struct QueueElementHeader header = {.type = QUEUE_REORDER};
};
```

#### Character's Turn

Server will ask to the controller to choose what character will do in the turn.
Will choose an eventer.

Now Character's Turn Queue Structure Definition:


```C
#define CHARACTER_REQ_ALIVE = (1<<1)

struct QueueCharacterTurn {
	struct QueueElementHeader header = {.type = QUEUE_CHARACTER};

	digits32 allowedEventerTypes;
	struct Character* character;

	digits32 requirements;
};
```

Energy and spell spending must be **before** executing the eventer executer!

In some turn types, server will only allow to some specific kind of eventers.
When an unavailable eventer has been called or eventer used with a irrelated target,
server will send a denied response and ask for again.

When an eventer has been choosen and can be used in this turn; server will
execute the eventer's function. Eventer's function should can access(rw) to
all the world and the character itself and the target, seperately.

Turn Types:

- Classical Turn: Can used in anything.
- Fast Magic Turn: Can only used in fast magics.
- Fast Combat Turn: Can only used in fast magicless combats.


#### Timed Effects

Every timed effect has its own proccess. But every timed effect is called from
a function that gets timed effect's variable.

```C
struct QueueTimedTurn{
    struct QueueElementHeader header = {.type = QUEUE_TIMED};

    struct TimedEffect* effect;
    void (*executer)(struct World*, struct TimedEffect* effect);
};
```

#### Entity Turns

The working segment is exactly same as character except the choosing an eventer. Entitiy
turns always does only one eventer. But that eventer can call different eventers.

```C
struct QueueEntityTurn{
    struct QueueElementHeader header = {.type = QUEUE_ENTITY};

    struct Entity* entity;
    void (*executer)(struct World*, struct Entity* entity);
};
```

### Stat Struct
This definition needed for explanation now on.

```C
struct Stats{
    int STR, DEX, CNS, WIS, SCS;
};
```


### Effect Struct and Rules of Effects
Effect Struct is a reference struct to effects. Every effect
shall follow these kind of header.

```C
#define EFFECT_STATGIVER (1<<0)
#define EFFECT_INVISIBLE (1<<1)

#define INVOKE_ON_REORDER (1<<0)
#define INVOKE_ON_CHARACTERTURN (1<<1)

#define EFFECTOR_PLAYER 0x1
#define EFFECTOR_DIMENSION 0x2

struct Effect {
	unsigned int ID;
	uint8_t effectorType;
	id_number effectorId; // player id if type is EFFECTOR_PLAYER, effector th dimension if
                    // EFFECTOR_DIMENSION is choosen

	int time;

	digits32 effectSpecs;

	struct Stats givenStats;

	digits32 invokeTimeType;

	struct Character* character;
	void (*executer)(void* effectptr, struct World*, struct Character* entity);
};
```

When a character is exiting from a dimension, server should search the dimension invoked
effects and remove them.

#### Example of a custom effect:

```C
// I want to create a three sectioned effect that does different things on each one.

struct CustomEffectDefinitionHere {
    struct Effect effect = {.time = 3, .invokeTimeType = INVOKE_ON_REORDER_TURN}; //header

    unsigned int segmentCounter = 0;
    void (*segment1)(void* effectptr, struct World*, struct Character* entity);
    void (*segment2)(void* effectptr, struct World*, struct Character* entity);
    void (*segment3)(void* effectptr, struct World*, struct Character* entity);
};

void CustomEffectDefinitionHere_executer(void* effectptr, struct World* world, struct Character* entity)
{
    struct CustomEffectDefinitionHere* effect = effectptr;
    if (effect->segmentCounter == 0) effect->segment1(effectptr, world, entity);
    else if (effect->segmentCounter == 1) effect->segment2(effectptr, world, entity);
    else if (effect->segmentCounter == 2) effect->segment3(effectptr, world, entity);
    effect->segmentCounter += 1;
}

```

Effects will do something again and again by the turn.

Some effects may give additional stats to the character, temporarely. These effects are
Stat Giver Effects. When a Stat Giver Effect adden to or removed from a character, ```stats```
variable will be calculated again.

### Eventer Struct

Eventer Struct is a reference struct to eventers. Every eventer
shall follow these kind of header. So a custom eventer can be
defined just like custom effects.

```C

#define EVENTER_CLASSIC (1<<0)
#define EVENTER_FASTMAGIC (1<<1)
#define EVENTER_FASTCOMBAT (1<<2)

#define TARGET_ONE (1<<0)
#define TARGET_AREA (1<<1)
#define TARGET_POSITION (1<<2)


struct Eventer {
	unsigned int eventerCode;
	id_number ID;

	int energy, spellEnergy;
	digits32 eventer_type, target_type;

	void (*executer)(void* eventer, struct World*, struct Character*, void* target, struct Tool* tool);
	int (*canCast)(void* eventer, struct Character*);
	void (*notChoosed)(void* eventer, struct World*, struct Character*);// if an eventer needs focus for couple of times, not choosing it
                                       // should be able to break the focus.
};
```


### AttackInfo Struct and Attacks
Attacks should send an info about the attack when tries to hit.

```C
struct Attackinfo;

typedef int (*HitterFunction)(void* hitting, struct Character* hitter, struct AttackInfo);
// hit functions should report whether the attack hitted or not

#define ATTACK_NONDODGEABLE (1<<0)

#define DAMAGE_BLUDGEONING 0x01

struct AttackInfo {
	struct Stats additiveStats;
	unsigned int specs;

	enum DamageType damageType;
	int damage;
};
```


### Character Struct

```C
#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

struct Character;
struct AttackInfo;

#define CHARACTER_DEFAULT 0x00
#define CHARACTER_DIO 0x01
#define CHARACTER_PAIN_DEVA 0x02

struct Character{
    unsigned int characterCode = CHARACTER_DEFAULT; // for determining the look
    id_number ID;

    int x,y;

    struct Stats baseStats;
    struct Stats stats;

    int maxhp, maxe, maxse;
    int hp, e, se;
    unsigned int state = 0;

    size_t passivePowerCount;
    struct PassivePower* passivePowers;

    size_t eventerCount;
    struct Eventer* eventers;

    struct List effects; // server needs to change effects fastly

    HitterFunction headHit, bodyHit, armHit, legHit;


    ControllerInterface* controllerInterface;
};
```

Every hit function should have a controlling segment that an attack is succesfull or not.
If the hit succesfully done, the hit functions should notify the server system.


### Dimensions

World has dimensions in it. In example Sasuke from Naruto anime/manga series can do dimension
travels or Funny Valentine from JoJo's Bizzare Adventure can travel parallel universes. So,
a dimension system is required for these types of powers. Some dimensions could do some
effects to the characters. In example in some dimensions, the air could be so heavy to breath
and that could make all the players to deacrease the energy regeneration or constantly deacreasing
it.

```C
struct CharacterListElement {
	struct ListElementHeader header;
	struct Character* character;
};

struct EntityListElement {
	struct ListElementHeader header;
	struct Entity* entity;
};

struct AreaListElement {
	struct ListElementHeader header;
	struct Area* entity;
};

struct Dimension {
	id_number ID;
	struct List characterList;
	struct List entityList;
	struct List Areas;
};
```

### Effect Areas

```C
struct Area {
    int x1, y1, x2, y2;

    void (*whenEntered)(struct Area*, struct Character* enterer);
    void (*whenExited)(struct Area*, struct Character* exiter);
    void (*whileInside)(struct Area*, struct Character* insider);
};
```

Are



### Observing Information

This information contains the world information that the character
can see. World information is the only required informations to
see or hear somethings.

```C
struct CharacterInformation {
	id_number ID;
	unsigned int characterCode;
	int x,y;
	int hp;
	float dirx,diry;
};

struct EntityInformation{
    id_number ID;
    unsigned int entityCode;
    int x,y;
    float dirx,diry;
};

struct ObservingInformation{
    id_number selfid;

    struct Stats characterStats;
    int hp, maxhp;
    int e, maxe;
    int se, maxse;
    unsigned int state;

    struct List effects;

    size_t eventerCount;
    struct Eventer* eventers;


    size_t characterCount;
    struct CharacterInformation* charInfos;

    size_t entityCount;
    struct EntityInformation* entityInfos;
};

struct ObservingInformation observe(struct Character* as, struct World* world);
```






### Controllers and Communications


#### Controller Communicating System (CCS)

Server should listen to the invoking client connection requests all the time.
When a client wants to join, server needs to accept it and then ask it which character
to choose in server. If there's no empty characters, controller should be able to
join as a new character aswell.

```C
struct Controller {
    int socket_fd;
    char* nickname;
};

struct ControllerCommunicatingSystem {
    struct List controllers;
} CCS;
```

##### Requests and Responses

```C
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
```


#### Controller Interface

Detailed informations about environment and other characters shall
pass from the perspective of a character to a communicator.

```C
struct ControllerInterface;

typedef void (*ControllerObserve)(struct ControllerInterface*, struct ObservingInformation*);
// and all the other eventer etc. things here please

struct ControllerInterface {
    struct Controller* controller;
    ControllerObserve observer;
};

void sendObservationToController(struct ControllerInterface*, struct ObservingInformation obsInfo);

struct ControllerInterface getDefaultControllerInterface(struct Controller* controller);
```

##### Custom Interface Example

This example will create a mind playing power's communicator that doesn't gives any information
(making the character feelingless) to controller.

```C
struct GenjutsuInterface{
    struct ControllerInterface interface;

    struct Character* caster;
};


void inGenjutsuObserver(struct Character* character, struct ObservingInformation* world)
{
    struct GenjutsuInterface* interface = character->controllerInterface;
    if(interface->caster->state & (STATE_DEAD | STATE_FAINTED)) {
        free(character->controllerInterface);
        character->controllerInterface = getDefaultControllerInterface();
        character->controllerInterface->observer(character, world);
    } else {
        struct ObservingInformation worldInfo = {.characterCount = 0,
                                .charInfos = malloc(0),
                                entityCount = 0,
                                .entityInfos = malloc(0)};
        sendObservationToController(interface, worldInfo);
        free(worldInfo.charInfos);
        free(worldInfo.entityInfos);
    }
}

struct ControllerInterface* getGenjutsuInterface(struct Character* caster)
{
    struct GenjutsuInterface* interface = malloc(sizeof(struct GenjutsuInterface));
    interface->interface.observer = inGenjutsuObserver;
    interface->caster = caster;
    return (struct ControllerInterface*)interface;
}
```

### TBWG Manager

All the game properties that explained above for server is designed to work with
TBWG Manager. This is the thing that manages all the game logic.

```C

struct tbwgdata {
	struct Queue queue;
	struct World world;
};


struct tbwgdata tbwgInit();

```

When the game starts, TBWG Manager should be inited.




