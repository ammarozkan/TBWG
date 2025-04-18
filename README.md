wanna break from the ads? skip to the BRO I JUST WANT TO PLAY segment.

# TBWG

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

# HOW AM I GONNA PLAY IT

Oh dear. That. That is cool. If you're gonna create the server, then you should follow
some making process.

## the TBWGLib

In the TBWGLib folder, run

```
make tbwgessentialslib lib
```

if you want to debug the lib

```
make tbwgessentialslib debug
```

(idk why would you want it though)

## the DEMO server and client

```
make outs serverdemo
```

if you want to do the debug one

```
make outs debugserverdemo
```

then you can run the demos with

```
./outs/tbwgserverdemo.out
```

```
./outs/tbwgclientdemo.out
```

## IIIIII WANT TO PROGRAM THE SERVER!!!

Gosh. Dont do that. If you really want to find out, read the [EXPLANATION](EXPLANATION.md). Then investigate the
[Test Main](TBWGLib/testmain.c) then the [Connetion Test](TBWGLib/tbwgcontest.c) then you could theoratically
write the server on your own (with the help of lib of course). Its kinda easy after you learnt it. You can add
your own character or effects with it.

## BRO I JUST WANNA PLAY

GOSH. STOP IT. OKAY. Really want to just play? Then find a server that runs somewhere. Idk from a friend maybe.
Or directly run this:

```
cd TBWGLib
make tbwgessentialslib lib
cd ..
make outs serverdemo
./outs/tbwgserverdemo.out
```

then you have server that is working. Then just connect with your client. Dah.

## Bro... I dont have linux.

Damn. Then you can't pla- NAH! IM GONNA CREATE A CLIENT THAT WORKS ON WINDOWS! OR YOU CAN TOOO!!!!
IMMA DO IT WITH PYTHON SOMEHOW! UNTIL THEN CYA! IF YOU DID IT, USE IT! PUBLISH IT GODDAMNIT!
