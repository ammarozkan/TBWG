### Explanation

This game is created to battle with a lot of creative powers.


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
};

#define ITERATE(list) for(struct ListElement* elm = list.firstelement; elm != NULL, elm = elm->next)

void addElement(struct ListElementHeader* list, void* element);

void removeElement(struct ListElementHeader* list, void* element); // we don't need the list actually.
```

## Server

Server creates queue, when a character's turn came up, server will send the information
that character gets and ask the controller to what to do for that character. Executes
effects when the Reorder Turn came up.

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
struct QueueCharacterTurn{
    struct QueueElementHeader header = {.type = QUEUE_CHARACTER};

    struct EventerTypes allowedEventers;
    struct Character* character;

    int characterShouldBeAlive;
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
    struct QueueElementHeader header = {.type = QUEUE_ENTITY};

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

### Character Struct

```C
#define STATE_DEAD (1<<1)
#define STATE_FAINTED (1<<2)

struct Character{
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
};
```

### Effect Struct and Rules of Effects


```C
#define EFFECT_STATGIVER (1<<0)

enum InvokeTimeType {INVOKE_ON_REORDER_TURN, INVOKE_ON_CHARACTER_TURN, INVOKE_EVERYTIME};

struct Effect {
    int time = 0; // if -1, effect is parmenent. else when time is 0, effect will be destroyed.
    unsigned int effectSpecs = 0; // for example effectSpecs = EFFECT_STATGIVER | ... for Stat Givers

    struct Stats givenStats;

    enum InvokeTimeType invokeTimeType;
    struct Character character;
    void (*executer)(struct World*, struct Character* entity);
};
```

Effects will do something again and again by the turn.

Some effects may give additional stats to the character, temporarely. These effects are
Stat Giver Effects. When a Stat Giver Effect adden to or removed from a character, ```stats```
variable will be calculated again.

### Eventer Struct

```C
enum EventerType {
    TYPE_CLASSIC, TYPE_FASTMAGIC, TYPE_FASTCOMBAT
};

enum TargetType {
    TYPE_TARGET, TYPE_AREA, TYPE_POSITION;
};

struct Eventer{
    int energy; int spellEnergy;
    enum EventerType eventer_type;
    enum TargetType target_type;
    void (*executer)(struct World*, struct Character*, void* target);
};

struct EventerTypes{
    size_t count;
    enum EventerType* types;
};

```
