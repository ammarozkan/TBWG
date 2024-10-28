### Explanation

This game rule standard is created to battle with a lot of creative powers.

# TBWG 1

TBWG is a Server Engine to serve a turn based battle with a lot of creative powers.
From stopping time to controlling gravity, from seeing really sharply to being
almostly invisible, from approaching from back to having great reflexes, from throwing
knifes to switching two seperated things, from teleportation to traveling across
dimensions etc.

# Game Logic


## Queues
First base logic of this game is the game queue.


Activities on queues:
#### Character Turns
When a turn of Character came up, that Character should make
the move.

In every turn, there could be limit of eventer type. And there will
be a count of spending costs. In example movement, fast attack, fast magic etc.

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

## Positions and Movement

(1,0) position means (0.50m, 0.0m)

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
Every character can be effected by some effects. These effects may invoked
differently. Invocation types are clock, turn, hit...

Clock/Tick: In every reorder turn, time passes some. And the clock ticks! Clock/Tick effects
shall be done.

Turn: When a character's turn came up, Turn effects shall be done.

Hit: When a character gets hitted, the Hit effects shall be done.



## Powers

### Passive Powers

Passive powers will change character's base queue, base stats or
add some effects to the character when the fight is starting.

In example karate fighter's speed power can add a "professional attack"
queue to fighter's base queue to make an additional karate attack.


### Active Eventers

These kind of eventers can be used in the turn. Every power has a specific
type. In example punch, karate straight punch, karate lunge punch are
fight powers. Every eventer has a energy spending type. Some eventers could have
a constant energy use, some eventers could have a dynamic energy use.

If a character uses a eventer that enough to spend to base energy requirements but
not the dynamic ones, character will spend the costs because of trying to work
this eventer.

## Events and Vision Levels

Every character has a vision level. And every event, entity or character has a vision
resistence. Basically, the vision resistance for entities and characters is zero. So
every character that has that thing in the vision area, can see it. But when a character
gains a effect that makes the vision resistence higher, then some characters may not
see it. In example, when a character is inside a bush area, low vision leveled characters
may not see it by the effect.

When a character does some hand sign to perform something, some characters may see what
sign the character is doing by checking the vision level of observer and vision hardness
of this event. In example sharingan users may see more clearly what the character is doing
in front of him before the sign is done and the thing is performing. (Thats called World
Events in the library. Look at the World Events section for detailed information.

```
distanceFactor = max(1.0f, pow(distance, 0.25f)/2.0f)
VisionHardnessFinal = VisionHardness * pow(distance, 0.25f)/2.0f
```

### Vision Resistence Standardization

- No Restriction : 0
- Crouching : 1
- Grass : 5
- Much More Grass like Forest or When Top of A Tree: 10
- Below the Ground : 70

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

## Core (Core to be the Server)

Core creates queue, when a character's turn came up, server will send the information
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

Core will add every character's base queue to the start of the queue in order
of character's speed.

```C
struct QueueReorderTurn {
    struct QueueElementHeader header = {.type = QUEUE_REORDER};
};
```

#### Character's Turn

Core will ask to the controller to choose what character will do in the turn.
Will choose an eventer.

Now Character's Turn Queue Structure Definition:


```C
#define CHARACTER_REQ_ALIVE = (1<<1)

struct QueueCharacterTurn {
	struct QueueElementHeader header;

	digits32 allowedEventerTypes;
	struct EventerUses gainingUses;
	struct Character* character;

	void (*whenInvoked)(struct QueueCharacterTurn*);

	digits32 requirements;
};
```

Energy and spell spending must be **before** executing the eventer executer!
(Or not. Events should detain their own spendings with the required information
(area, position, direction informations in example).)

In some turn types, server will only allow to some specific kind of eventers.
When an unavailable eventer has been called or eventer used with a irrelated target,
server will send a denied response and ask for again.

When an eventer has been choosen and can be used in this turn; server will
execute the eventer's function. Eventer's function should can access(rw) to
all the world and the character itself and the target, seperately.

In every new character turn, character's spendings will be setted to gainingUses
Every eventer will spend a use. If the character don't has any more turns,
then character can't use the eventer. After the character turn, if character gets
an attack and tries to avoid it, auto-avoidation should use the remaining spendings.

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
struct Stats {
	int STR, DEX, CNS, WIS, SCS;
	float visionAngle; // in radians (maximum 2.8f for normal people (approximately) )
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

#define EFFECT_TRIGGER_TYPE_COUNT 3
#define EFFECT_TRIGGER_TYPE_CLOCK 0x00
#define EFFECT_TRIGGER_TYPE_TURN 0x01
#define EFFECT_TRIGGER_TYPE_HIT 0x02

struct Effect {
	unsigned int ID;
	uint8_t effectorType;
	id_number effectorId; // player-

	int time;

	digits32 effectSpecs;

	struct Stats givenStats;

	digits32 invokeTimeType;

	struct Character* character;
	void (*executer)(void* effectptr, struct World*, struct Character* entity, void* relativeInformation);
};
```

System somehow should have hit, clock, turn effects. First option is store the effect structs seperately
for each triggering type. This will gain a bit memory and CPU performance. Second option is store all of the
structs in one list and seperate them with a uint8_t value. Then when a triggering needed, a function like
```triggerEffects(EFFECT_TRIGGERTYPE_TICK, effectList, character)```. The second option will be less performance
wise for CPU and maybe for memory. But will be so much more modular. Or, the first option could be done with
a new style for modularity. So new style may be needed to finded out.


Oh! The character struct could have a struct Effect effects[EFFECT_TYPE_COUNT];!!!

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


Other types of effect needs to be stored differently.


### Eventer Struct

Eventer Struct is a reference struct to eventers. Every eventer
shall follow these kind of header. So a custom eventer can be
defined just like custom effects.

```C

#define EVENTER_TYPE_CLASSIC (1<<0)
#define EVENTER_TYPE_FASTMAGIC (1<<1)
#define EVENTER_TYPE_FASTCOMBAT (1<<2)


#define TARGET_ONE (1<<0)
#define TARGET_AREA (1<<1)
#define TARGET_POSITION (1<<2)

#define EVENTER_REQUIRED_INFORMATION_POSITION (1<<0)
#define EVENTER_REQUIRED_INFORMATION_AREA (1<<1)
#define EVENTER_REQUIRED_INFORMATION_DIRECTION (1<<2)

#define EVENTER_DEFAULT 0x00

struct EventerUses {
	unsigned int classic, fastmagic, fastcombat, thoughtmagic, movement;
};

void addEventerUses(struct EventerUses* a, struct EventerUses b);
int checkRequiredEventers(struct EventerUses, struct EventerUses req);

struct EventerRequiredInformations {
	iVector position;
	iVector position2;
	fVector direction;
	iVector A,B; // area coordinates A and B
};

int executerCanExecuteNow(void* eventer, struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool);

#define EVENTER_ENERGYUSE_BASE 0x01
#define EVENTER_ENERGYUSE_BYUSE 0x02

typedef int (*tbwgtypeSetEventerReady)(struct Eventer*, struct Character*, struct World*);

struct Eventer {
	unsigned int eventerCode;
	id_number ID;

	char name[32];

	uint8_t energyValueType;
	int baseEnergy, baseSpellEnergy;
	digits32 eventer_type, required_informations;
	struct EventerUses costs;

	tbwgtypeSetEventerReady setReady;

	int (*canExecutedNow)(void* eventer, struct World*, struct Character*,
		struct EventerRequiredInformations, struct Tool* tool);

	void (*executer)(void* eventer, struct World*, struct Character*,
		struct EventerRequiredInformations, struct Tool* tool);

	// when not choosed

	void (*notChoosed)(void* eventer, struct World*, struct Character*);
};
```

When a eventer is a selectible option, in example when the turn of character cames up, the setReady
function will be called by system. With this, values of eventer may be changed before communicating
with the interface or using costs by the desire of this eventer. In example, lets say this is a
magic that required some hand signs first to get that ready. Lets say hand signs wont require any spell energy.
Then in the first execution, this eventer shouldn't require any spell energy. Then in the second execution,
this eventer should require some spell energy and some Magic Eventer Use cost. This kind of thing can be done
with that function.

When a eventer has EVENTER_ENERGYUSE_BASE in energyType, then the baseEnergy values will be
the constant energy using values.

When a eventer has EVENTER_ENERGYUSE_BYUSE in energyType, then the baseEnergy values will be the
minimum spending of this eventer. After spending them, the getEnergyUse functions should be executed.


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

#define CHARACTER_DEFAULT 0x00

typedef int (*SeeCharacter)(struct Character* observer, struct Character* target);
typedef int (*SeeWorldEvent)(struct Character* observer, struct WorldEvent* target);
typedef int (*CanSeen)(struct Character* observer, struct Character* target);

struct Character {
	TBWGType tbwgType;

	unsigned int characterCode;
	id_number ID;

	iVector position;
	struct Dimension* dimension;
	fVector direction;

	struct Stats baseStats;
	struct Stats stats;

	iValue hp, e, se;
	digits32 state;

	struct List seeingResources; // in example gaara's eye, rinnegan bodies.

	size_t passivePowerCount;
	struct PassivePower* passivePowers;

	size_t eventerCount;
	struct Eventer* eventers;

	struct List effects;

	struct Queue baseQueue;

	HitterFunction headHit, bodyHit, armHit, legHit;

	struct ControllerInterface* controllerInterface;

	SeeCharacter seeCharacter;
	SeeWorldEvent seeWorldEvent;
	CanSeen canSeen;
};
```

Every hit function should have a controlling segment that an attack is succesfull or not.
If the hit succesfully done, the hit functions should notify the server system.


### Entities

Rocks, rasenshrukien, Dio's knifes, Gojo's blue and red BAlLs... All these are required to
be existed as entities.

```C
struct Entity {
	TBWGType tbwgType;

	digits32 entityCode;
	int x,y;
	float dirx, diry;
	float mass;

	CollisionFunction collisionFunction;

	HitterFunction hit;

	CanSeen canSeen;
};
```

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

    void (*whenEntered)(struct Area*, struct Being* enterer);
    void (*whenExited)(struct Area*, struct Being* exiter);
    void (*whileInside)(struct Area*, struct Being* insider);
};
```
The tbwgAddArea function will be used for adding the area to the system. this function will
control who is inside of the area (needs to be generalized to beings rather than characters)
and executes whenEntered() for those that are inside.

When a position change happens, the tbwg system will be controlling which areas needs to execute
whenExited() or whenEntered().

When reorder queue executed, every areas whileInside() function shall be executed to the beings
that are inside the area.

May all position changes should be done by the tbwg system rather than effects manually doing that.
tbwg system could give an interface to effects to handle it. In example public tbwgMoveBeing(struct Being*)
function.


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

struct EntityInformation {
    id_number ID;
    unsigned int entityCode;
    int x,y;
    float dirx,diry;
};

struct ObservingInformation {
    id_number selfid;

    struct Stats characterStats;
    iValue hp, e, se;
    digits32 state;

    struct List effects;

    size_t eventerCount;
    struct Eventer* eventers;


    size_t characterCount;
    struct CharacterInformation* charInfos;

    size_t entityCount;
    struct EntityInformation* entityInfos;
};

typedef int (*SeeCharacter)(struct Character* observer, struct Character* target);
typedef int (*CanSeen)(struct Character* observer, struct Character* target);

struct ObservingInformation Observe(struct Character* as, struct World* world);
```

In observe function, all the character's self informations (except the invisible
effects) will be observed. For all the characters that in the vision angle the vision
functions will be executed (seeCharacter and canSeen functions in character struct).
And for other entities too.


#### Observing World Events

When a eventer does something that could seen by other entities or characters, that thing should
stream a world event. By calculations, the world event will be seen by the characters that could
see it.

```
struct WorldEvent {
	char* name;
	float visionHardness;
	float disappearingSpeed;
	iVector position;
};

struct WorldEvent getDefaultWorldEvent(char* name,
	float visionHardness,
	float disappearingSpeed,
	iVector position);

...

void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event);
```



### Controllers and Communications


#### Controller Communicating System (CCS)

Core should listen to the invoking client connection requests all the time.
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


struct tbwgdata* tbwgInit();
void tbwgUse(struct tbwgdata* data);

// unique events
void tbwgTurn();
void tbwgReorder();
void tbwgInterruptTurn(); // will be used when a stopper trap invoked
void tbwgMakeObserveAllCharacters();
void tbwgStreamWorldEvent(struct Dimension* dim, struct WorldEvent event);

void tbwgTriggerEffects(unsigned int effectType, void* relativeInformation);
void tbwgAreaWhileInsides();


// setters
int tbwgAddCharacter(struct Character* character);
int tbwgAddArea(struct Dimension* dim, struct Area* area);

// getters
struct World* tbwgGetWorld();
struct Dimension* tbwgGetFirstDimension();
struct Dimension* tbwgFindDimensionByPos(unsigned int pos);
struct Dimension* tbwgFindDimensionByID(id_number ID);

// changers
void tbwgMoveBeing(struct Being*, iVector positionChange);
void tbwgPutBeing(struct Being*, iVector position);
```

When the game starts, TBWG Manager should be inited with tbwgInit(). If more than one tbwgs will be
inited, returned pointer of tbwgInit should be saved and the tbwgUse function should be used for
switching the tbwg datas.

tbwgReorder does the reordering.

tbwgTurn pops the next thing on queue list and executes it.

tbwgInterruptTurn created to be used in effects, eventers, areas etc. Interrupts the current
turn. So the character's, that is making decisions, turn will be interrupted.

tbwgMakeObserveAllCharacters will make all characters observe the what they can observe.

tbwgStreamWorldEvent created to be used in effects, eventers, areas etc. Streams a world event
to hint some information about the movement.



## Communication (Server, Client)


A communication standard should be defined to work with this type of fight game.
I'll call the defined standard as **TBWGCON 1**.

Before getting to it, some section may include somethings like (CP. x) and some structs include something like
```unsigned int nextchapter;``` Those are the client manipulators. Client manipulators are being used to return
to some segment in the process. aka JMP instructor.

### Joining

Clients will want to join the game in any time. That could be before the game starts, while in game, or after the game.

When a client tries to join, should make the first entrance with the checking pkg. (CP. 1)

```C

struct CheckingPackage {
	char[4] tbwgname; 	// server standard name, defaultly "TBWG". that may changed by the creator when the creator is
						// creating a custom server.
	unsigned int version[3]; // v[0].v[1].v[2] = MAJOR.Minor.patch
};

```

Then server responds with the closest server type and the version to introduced pkg. if the pkg is not supported
by the server, otherwise server responds with the error code 0.

```C

struct WelcomingPackage {
	struct CheckingPackage ip; // same as client sended one if supported by server, otherwise closest to that
	unsigned int errcode; // will be used as .errcode = UNSUPPORTEDVERSION | UNSUPPORTEDGAME;
	unsigned int nextchapter; // what should client do?
};

```

If server is okay with the version, client sends a name(may nickname). (CP. 2)

```C

struct IntroducementPackage {
	unsigned int nameSize; // max 32
	char name[nameSize]; // max 32 sized
};

```

Server responds.

```C

struct IntroducementResponse {
	unsigned int errcode;
	unsigned int nextchapter;
};

```

Client continues to reading if no error has been maden. (CP. 3)

```C

struct TBWGInformator {
	uint16_t characterCount;
	struct TBWGCONCharacterInfo charinfo[characterCount];
};

```

Client sends the choosen character.

```C

struct CharacterSelection {
	int selection; // if this selection is -1, client selects to create a character.
};

```

If client choosed an existing character, but that one wasn't available:

```C

struct CharacterSelectionError {
};

```

If client choosed a character is available, then we can say connection is succesfully done for this standard.

If client choosed to create a character, then we continue on **Character Creation** section.
