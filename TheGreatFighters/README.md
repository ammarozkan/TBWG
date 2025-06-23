# TheGreatFighters
is a test server/game to test the abilities of TBWG system and TBWGLib


# Eventers

## TgfLook (1 E, 0 SE) (1 movement)

Gets direction, looks to that direction.

[(0.0f, 0.5f) MVM_LOOK]

## TgfWalk (2 E, 0 SE) (1 movement)

Can make a walk SPD * 0.1 m away.

[(0.7f, 0.5f) SND_WALK] on starting location
[(0.7f, 0.5f) SND_WALK] on target location
[(0.7f, 0.5f) MVM_WALK] on actor

## TgfPunch (6 E, 0 SE) (1 fastcombat)

Punch to somewhere closer than 0.75m.
If there's a character, 5*STR bludgeoning will
be given. In chance of 10/60, damage will be increased
by STR.

[(0.5, 0.5) MVM_PUNCH] on actor
[(3.0, 0.5) SND_MVM_PUNCH] on actor
[(0.5, 0.5) SND_PUNCH] on actor

## TgfRest (0 E, 0 SE) (1 classic)

Just waits to restore CNS/2 energy.
if energy is full, then other restorings
will be go to the health.

## TgfPush (8 E, 0 SE) (1 movement, 1 fastcombat)

Pushes a target that its in the walkable distance,
taking down its place, giving STR/2 bludgeoning
damage and giving the TgfPuzzled effect for 1.5 seconds.
By 10/70 chance, damage could increase by STR.

[(0.1, 0.5) ACT_PUSHED] on target actor
[(0.1, 0.5) MVM_PUSH] on actor
[(1.0, 0.5) SND_PUSHED] on target actor
[(1.0, 0.5) SND_WALK] on actor
[(1.0, 0.5) SND_WALK] on target actor

## TgfGrab (5 E, 0 SE) (1 fastcombat)

Grabs a character near 0.75 m.

[(0.1, 0.5) MVM_GRAB] on actor
[(3.0, 0.5) SND_MVM_GRAB] on actor
[(0.1, 0.5) MVM_GRAB_HAND] on target
[(0.1, 0.3) MVM_GRABBED] on target actor
[(0.1, 0.3) SND_GRABBED] on target actor

## TgfGrabWalk (5 E, 0 SE) (1 movement)

Walks, grabbing the grabbed one. If position changes
without this one, grabbing will be breaked.

[WorldEvents are same with walking.]

## TgfSilentWalk (1 E, 0 SE) (1 movement) (2 actual movement)

Two stepped walking eventer.

Get ready,

[(0.7, 0.5) MVM_SILENT_WALK] on actor
[(1.2, 0.5) SND_WALK] on actor

then
 
[(0.7, 0.5) MVM_SILENT_WALK] on actor
[(1.2, 0.5) SND_WALK] on actor

## TgfTrickyPunch (11 E, 0 SE) (1 fastcombat)

Tricky punch that attacks with +3 DEX.

by 10/60 chance, hits to the head. If the hit to the
head is increased STR attack, then TgfPuzzled will be
given for 1 seconds.

[(0.5, 0.5) MVM_TRICKY_PUNCH] on actor
[(3.0, 0.5) SND_MVM_TRICKY_PUNCH] on actor
[(0.5, 0.5) SND_TRICKY_PUNCH] on target
[(0.3, 0.5) SND_TRICKY_CRITIC] on target when critic on head

## TgfGetUp (1 E, 0 SE) (1 movement)

Gets up the character.

# Characters

## DefaultHuman (100/100 HP, 40/40 E, 0/0 SE) (10STR 10DEX 10CNS 10WIS 0SCS 6SPD 2VISRES)

- Eye: 1.8r, (1.0, 0.1) vision, (1.0, 0.3) hearing

Has:
- TgfPunch
- TgfWalk
- TgfLook
- TgfRest


## StreetFighter (140/140 HP, 40/50 E, 0/0 SE) (12STR 13DEX 14CNS 10WIS 0SCS 6SPD 2VISRES)

- Eye: 1.8r, (1.0, 0.1) vision, (1.0, 0.3) hearing

Has:
- TgfPunch
- TgfWalk
- TgfLook
- TgfRest
- TgfPush
- TgfGrab





# Effects

## TgfPuzzled

Slows down the enemy.

Stats = -1 STR, -2 DEX, -1 SCS, -1 SPD

## TgfGrabbing

if target's or user's position changes, effect removes on clock with [(0.7, 0.3) SND_BREAK_GRABBING].