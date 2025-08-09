# TBWG

wanna break from the ads? skip to the I JUST WANT TO PLAY segment.

TBWG is a Turn Based War Game. I want to give players almostly full freedom to do what they want.
Game can be modified easily.

#### Quick Notes

- Entities are not fully implemented, yet.

## Logic

So theres characters. Characters have a base turn. When a special type of turn hits from the queue,
named "Reorder Turn" or theres not turn to be execute, every character's base turn is added to the game. 
So they can make some actions. Base turns could contain more than one turn, special type of turns, or 
even autoturns (idk). For example items or objects in the game are doing autoturns. Flying for example. 
Throwing something.

Every character turn has an "eventer use" amount. Thats kind of a speed value. In a turn, a character
can do a "classic" move. 1 classic move equals to 1 arm movement, 1 hand movement, 1 movement movement
(or legs), 1 thought movement. A punch would use 1 arm and 1 hand for example. If a walk uses 1 movement,
character can do a walking punch in a turn or round.

## Actions

You can program actions to make somethings. Maybe fascinating things. For example, creating a area 
that teleports someones to somewhere. (idk)



# Compiling a Server

A basic server of TBWG uses the TBWGLib. I'd use compiling TBWGLib seperately and using that
compilement for server compilation via linking to use the symbols.

## Compiling the TBWGLib

On linux, GCC and MAKE required. and also linux socket.h ... and also GNU C lib..

On windows, just MinGW.

### Linux

In the TBWGLib directory, running

```bash
make tbwgessentialslib lib
```

will compile the lib onto a directory named "libs". If debug or some interesting stuff
is wanted to compile too, see the Makefile.

then, unnecessary things could be cleaned up

```bash
make clean
```

### Windows

In the TBWGLib directory, theres a .bat file to run. Double clicking or executing
it from terminal will compile the lib into a directory named "libs".

Compiled libs:

- libtbwg.a
- libtbwgdebug.a
- libtbwgessentials.a
- libtbwgnet.a
- libtbwgnetdebug.a

it will cost approximately 285 KB of storage.

## Compiling the Server Scheme



# I JUST WANT TO PLAY

just add the files that are outputten from a TBWGDF_Compiler for the client
to the pythontbwgclient then run

```bash
python . {serveraddresshere}
```

and client will be opened and will try to join the server

# How to serve a server

tbwgnet for server is supported by windows and linux. on a compatible device, you can natively run a server.

## SERVER DEMO

firstly compile the tbwglib.

after, on the main directory

```bash
make serverdemo
```

then you should have a server on the /outs directory.

I haven't written a method for quickly compiling it on windows.

## TheGreatFighters

TheGreatFighters is a complete server program/game to test the strength of TBWG. It is on a different GitHub
page.