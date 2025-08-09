This is an auto code generating framework that allows some
typos to quickly write Effects, Eventers, Characters and Entities


## Important

All the code must be written in "C" as it will output things in C
and some of Python for Client side code.


## Where to put files and how to define things?

Compiler will choose a directory to recursively read and convert
files. So its not that necessary to put "where". Effect, eventer
and other definition could be seperated or not. It will read all
of the files like its a single file. Order is not important.
Compiler; reads and saves the informations first, then compiles
by according to those informations. So the programmer can define
a eventer later but use it on an earlier defined character.


### Basics

#### Change Invoke

Lets say we defined a character.

```C
@character CharacterName {};
```

And putted this line after it.

```C
@DEX "13"
```

What will happen? Basic! Character's dexterity will be 13. So those are change
invokers. They change something in the previously defined stuff.

```C
@effect Puzzled {};

@STR "-1" @DEX "-2" 
@SCS "-1" @SPD "-1"
@time "3"
```

Excellent! That Puzzle effect can puzzle with reducing those things
for 3 rounds!

Some explanations could be putten also for seeing explanations
from client, when the generated client related code implemented 
to the client.

```C
@character JohnLennon {};

@explanation "very respected, great, great musician."
```

and if the multilanguage thing is opened on the compiler

```C
@character JohnLennon {};

@explanation "very respected, great, great musician."
@explanationtr "çok saygı duyulan, büyük, büyük müzisyen."
@explanationfr "un tres respecté ..."
```

#### Steal

If you want to steal a function from another definition, enter a 
stealing command!

```C
@steal StuffA StuffB StolenFunction
```

This line, steals the function ```StolenFunction``` from StuffB to
StuffA.

#### Implementing

Every defined thing could be used for implementing a function!!
For that

```C
@implement TheNameOfThatThing usenVariable
void functionNameHere()
{
    // usenVariable will be the pointer, pointing to the Thing
}
```

Constructor, Executors can be implemented with that.

And also a quick implementation to a previously defined 
thing could be done with "_" sign.

```C
@character TheGreatCharacter {};

@implement _ character
void Constructor() {} // constructor of TheGreatCharacter
```

#### Quick Filters

If theres an object, and object has a interesting value that could be usen in the code;

```C
InterestingValueName@ObjectName
```

could be written anywhere of the code!

For example; effects, characters and eventers all have a unique code.


### Defining a Character

on a file

```C
@character CharacterName {};
```

defines a character. And with that, character is usable.

#### System Called Implementations

```C
@implement CharacterName character
void Constructor() {} // executed after allocating and setting the values
```




### Defining an Eventer

on a file

```C
@eventer EventerName {};
```

#### System Called Implementations

```C
@implement EventerName eventer
void Executer(struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool) {} 
                // the eventer executer function

@implement EventerName eventer
void Constructor() {} 
                // executed after allocating and setting the values

@implement EventerName eventer
int SetReady(struct Character* c, struct World* w) {} 
                // before the eventer selection call, this function will be called by the system.
                // if the function returns 0, system wont send the eventer to the client and eventer
                // will not be usable. if returns 1, its ok. and also changing of energy and other values
                // is possible as thats just being another function ;p

@implement EventerName eventer
void NotChoosed(struct World* world, struct Character* character) {} 
                // called when eventer is not choosed by the player.

@implement EventerName eventer
int CanExecutedNow(struct World* world, struct Character* user, struct EventerRequiredInformations reqinf, struct Tool* tool) {}
                // called when player choosed, and putted the required informations. if returns 1, it will spend the
                // energy and will call the executer. if returns 0, it wont spend the base energy and it wont call
                // the executer.
```

mmm.

### Using Raw Source and Header Files


So when a file is createn with tbwgc extension, it will be passed from these filter-like compilation
proccesses. But when person wants to write non-filtered (almost, I'm gonna talk about that.) code,
a c or h extensioned files could be created. In those files, what is written is what is outputted.
But for the sake of easy code assignment, support of Code@ is still goes.

Compilation is done like that (in the order that is written):



(Header convertion of the stuff that is written in .tbwgc files) +
(Raw Header Files) +
(Raw Source Files) +
(Source convertion of the stuff that is written in .tbwgc files) => morsels.h

So with that order, programmer could define a function in a source file without a header
and use it on the .tbwgc files.