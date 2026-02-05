# TBWG

TBWG is a Turn Based War Game. I want to give players almostly full freedom to do what they want.
Game can be modified easily.

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



# Compiling

A basic server of TBWG uses the TBWGLib. I'd use compiling TBWGLib seperately and using that
compilement for server compilation via linking to use the symbols.

## Compiling the TBWGLib

### Linux
Requirements: GCC, MAKE

In the TBWGLib directory, running

```bash
make tbwgessentialslib lib
```

will compile the lib onto a directory named "libs". If debug or some other interesting stuff
is wanted to compile too, see the Makefile.

then, unnecessary things could be cleaned up

```bash
make clean
```

### Windows
Requirements: MinGW

In the TBWGLib directory, theres a .bat file to run. Running
it will compile the lib into a directory named "libs".

Compiled libs:

- libtbwg.a
- libtbwgdebug.a
- libtbwgessentials.a
- libtbwgnet.a
- libtbwgnetdebug.a

it will cost approximately 285 KB of storage.

## Compiling the Server Template
Requirements: same as TBWGLib requirements + the compiled TBWGLib on ../TBWGLib

Compiled binary will be putten to ../bin. In any errors, try creating bin directory
manually.

### Linux

```bash
make classic
```

### Windows
Run the MakeWindows.bat


# Using TBWGDF_Compiler

## Informative Warning About a Failure

If you really care about running, read.

Due to some version mixing (I mistakenly merged two different versions of the code)
TBWGDF_Compiler does not compile correctly. It will bombard a lot of errors when tried
to compile the compiled C code. It should be able to fixed by analyzing.

## Using TBWGDF_Compiler

For compiling a proper Project folder with TBWGDF_Compiler, files
should look like this:

```
- The Great Folder
|--- TBWGDF_Compiler/
   |--- __main__.py
   |--- CPreProcessing.py
   |--- CTokens.py
   |--- ...
|--- Project/ (the game project folder)
   |--- Assets/
   |--- Codes/
   |--- Templates/
|--- TBWGLib/ (the compiled TBWGLib folder)
   |--- include/
   |--- src/
   |--- tbwgnetsrc/
   |--- Makefile
   |--- ...
|--- outcl/ (copy of tbwgclientpython)
   |--- tbwgplayer/
   |--- assets/ (copy of Project/Assets here)
   |--- __init__.py
   |--- __main__.py
   |--- ...
|--- outsv/ (copy of TBWGServerTemplate)
   |--- details/
   |--- morsels/
   |--- main.c
   |--- ...
```

then run the TBWGDF_Compiler with python. It will detect the folders by
their names. So NAMES SHOULD EXACTLY MATCH.

```bash
python TBWGDF_Compiler
```

Then for running the server, compile outsv by "Compiling the Server Template"
and run it from bin folder. 

For running the client, directly run outcl by

```bash
python outcl
```
