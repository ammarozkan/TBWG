# TBWG

wanna break from the ads? skip to the [BRO I JUST WANT TO PLAY](README.md#BRO I JUST WANT TO PLAY) segment.

TBWG is a Turn Based War Game. I want to give players almostly full freedom to do what they want.
So I'm using a Dungeons and Dragons style of game. You can even modify the game to add
actions you want.

## Logic

So theres characters. Characters have a base turn. When a special type of turn hits from the queue,
named "Reorder Turn", every character's base turn is added to the game. So they can make some actions.
Base turns could contain more than one turn, special type of turns, or even autoturns (idk). For example
items or objects in the game are doing autoturns. Flying for example. Throwing something.

## Actions

You can program actions to make somethings. Maybe fascinating things. For example, creating a area 
that teleports someones to somewhere. (idk)

# Compiling the TBWGLib

Basic compile is will be done by while on TBWGLib directory

```bash
make tbwgessentialslib lib
```

after, unnecessary things could be cleaned up

```bash
make clean
```

# HOW AM I GONNA PLAY IT

just modify the tbwgclientpython how you like it then while in the directory run

```bash
python .
```

and client will be opened and will try to join the server

# How to serve a server

tbwgnet for server is only supported by linux. on a linux device, you can natively run a server.

## SERVER DEMO

firstly compile the tbwglib.

after, on the main directory

```bash
make serverdemo
```

then you should have a server on the /outs directory

## TheGreatFighters

TheGreatFighters is a complete server program/game to easily add characters, objects and effects
to "the game". So its basically a game made with TBWG.

### How to compile TheGreatFighters

AFTER compiling the TBWGlib go to the /TheGreatFighters directory
and run 

```bash
make tgf
```