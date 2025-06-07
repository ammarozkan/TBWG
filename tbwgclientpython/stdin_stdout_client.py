import tbwgcon1

def characterselector(chrs):
    for c in chrs:
        print(f"{c}")
    return 1

def observe(obshdr : tbwgcon1.TBWGObservingInformationHeader, observation : tbwgcon1.TBWGObservingInformation):
    print(f"Observation for {observation.selfid}:")
    print(obshdr)
    print(observation)
    for eventers in observation.eventers:
        print("\t",eventers)
    for chars in observation.characters:
        print("\t",chars)

def worldevent(wevnt : tbwgcon1.TBWGWorldEventInformation):
    print(wevnt)

def eventer(evntrhdr : tbwgcon1.TBWGEventerOptionsInformationHeader, eventeroptinfos : tbwgcon1.TBWGEventerOptionsInformation):
    print(evntrhdr)
    print(eventeroptinfos)
    i = 0
    for eventer in eventeroptinfos.eventers:
        print(f"{i} : ",eventer)
        i+=1

    g = tbwgcon1.TBWGEventerRequiredInformations()
    com = input("[play:pass]:")
    if com == "play":
        return tbwgcon1.TBWGConTurnPlay(int(input("[eventernumber]:")), g, 0)
    else:
        return tbwgcon1.TBWGConTurnPlay(0, g, tbwgcon1.TURNPLAY_END_TURN)