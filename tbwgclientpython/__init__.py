import tbwgplayer as Player
import tbwgcon1

def program(viewerManager = None, tbwgconenable = True, ip="127.0.0.1", port=5005, address="google.com", widenet=False, name="John"):

    Player.init()
    player = Player.TBWGPyGamePlayer(1280, 720, viewerManager)
    player.characters = [tbwgcon1.TBWGCharacterInformation(6, 32, tbwgcon1.TBWGiVector(0,2), tbwgcon1.TBWGfVector(0.0,1.0), tbwgcon1.TBWGiValue(6,10))]


    def observe(obshdr : tbwgcon1.TBWGObservingInformationHeader, observation : tbwgcon1.TBWGObservingInformation):
        for c in observation.characters:
            print(c)
        player.characters = observation.characters
        player.observer = (observation.position, observation.hp, observation.direction)
        
        player.energy = (observation.e.value, observation.e.max)
        player.spellenergy = (observation.se.value, observation.se.max)

        player.oneffects = []
        for ef in observation.effects:
            player.oneffects.append(ef)

    def worldevent(wevnt : tbwgcon1.TBWGWorldEventInformation):
        player.addWorldEvent((wevnt.eventname,(wevnt.position.x,wevnt.position.y)))

    def eventer(evntrhdr : tbwgcon1.TBWGEventerOptionsInformationHeader, eventeroptinfos : tbwgcon1.TBWGEventerOptionsInformation):
        player.setRestUses([evntrhdr.eventeruses.classic,evntrhdr.eventeruses.arm,evntrhdr.eventeruses.hand,evntrhdr.eventeruses.movement,evntrhdr.eventeruses.thought])

        for evn in eventeroptinfos.eventers:
            print(evn)

        eventer_th = player.request(Player.REQ_EVENTER, eventeroptinfos.eventers)
        
        reqinfs = tbwgcon1.TBWGEventerRequiredInformations()
        
        specs = 0
        if eventer_th == None or eventer_th == -1:
            specs = tbwgcon1.TURNPLAY_END_TURN
            return tbwgcon1.TBWGConTurnPlay(0, reqinfs, specs)
        
        evn = eventeroptinfos.eventers[eventer_th]

        if evn.requiredinformations[tbwgcon1.REQ_POSITION]:
            x,y = player.request(Player.REQ_POSITION, eventeroptinfos.eventers)
            reqinfs.position = tbwgcon1.TBWGiVector(x,y)
        elif evn.requiredinformations[tbwgcon1.REQ_DIRECTION]:
            x,y = player.request(Player.REQ_DIRECTION)
            reqinfs.direction = tbwgcon1.TBWGfVector(x,y)
        t = tbwgcon1.TURNPLAY_END_TURN
        
        return tbwgcon1.TBWGConTurnPlay(eventer_th, reqinfs, specs)
    
    def characterselector(chrs):
        chrs_forplayer = []
        for chr in chrs:
            chrs_forplayer.append( (chr.code,) )
        return player.request(Player.REQ_CHARACTER_SELECTION, chrs_forplayer)
    
    def TBWGCON():
        import stdin_stdout_client as SSC

        if widenet:
            client = tbwgcon1.getClientByAdress(address)
        else:
            client = tbwgcon1.TBWGConClient(ip,port)

        client.setname(name)
        client.setcharacterselector(characterselector)
        if not client.connect():
            print("Some error happened while connecting")

        while client.step(observe, worldevent, eventer):
            pass

    t = None
    if tbwgconenable:
        import threading
        t = threading.Thread(target=TBWGCON)
        t.start()

    player.loop()

    if tbwgconenable:
        t.join()


def getTGFAssets(path):
    worldevents = f"{path}images/worldevents/"
    assets = Player.Assets()
    assets.addBackground("default",f"{path}images/frutigeraerobckr.webp")
    assets.addCharacter(0, f"{path}images/human2.png")
    assets.addCharacter(2, f"{path}images/fighter.png")
    assets.addCharacter(123, f"{path}images/avatar.png")
    assets.addEventer("TGF_PUNCH", f"{path}images/punch.jpg")
    assets.addEventer("TGF_LOOK", f"{path}images/look.png")
    assets.addEventer("TGF_WALK", f"{path}images/walkingman.jpg")
    assets.addEventer("TGF_REST", f"{path}images/resting.jpg")
    assets.addEventer("TGF_STFGHT_PUSH",f"{path}images/push.png")
    assets.addEventer("TGF_STFGHT_GRAB",f"{path}images/grabbing.png")
    assets.addEventer("TGF_STFGHT_GRABWALK",f"{path}images/carryingwalk.png")
    assets.addEventer("TGF_STFGHT_SILENTWALK",f"{path}images/silentwalk.webp")
    assets.addEventer("TGF_STFGHT_TRICKYPUNCH",f"{path}images/trickypunch.png")
    assets.addEventer("TGF_STFGHT_GETUP",f"{path}images/getup.png")
    assets.addEventer("TGF_STFGHT_GETDWN",f"{path}images/liedown.webp")
    assets.addEffect(16, f"{path}images/puzzled.png")
    assets.addEffect(17, f"{path}images/grabbing.png")
    assets.add("CancelButton", f"{path}images/please.gif")
    assets.add("DefaultButton",f"{path}images/salutman.webp")
    assets.add("DefaultRestUses",f"{path}images/token.png")
    assets.add("ClearWorldEvents",f"{path}images/clear.png")
    assets.add("HealthIcon",f"{path}images/heart.png")
    assets.add("RestUses_classic",f"{path}images/timeicon.jpg")
    assets.add("RestUses_armMove",f"{path}images/movementicon.png")
    assets.add("RestUses_handMove",f"{path}images/hand.png")
    assets.add("RestUses_movement",f"{path}images/leg.png")
    assets.add("RestUses_thought",f"{path}images/thoughticon.png")

    assets.addWorldEvent("MVM_LOOK", f"{worldevents}rotatinghead.png")
    assets.addWorldEvent("MVM_PUNCH", f"{worldevents}walkingsound.png")
    assets.addWorldEvent("SND_PUNCH", f"{worldevents}punchsoundicon.png")
    assets.addWorldEvent("SND_WALK", f"{worldevents}walkingsound.png")
    assets.addWorldEvent("MVM_WALK", f"{worldevents}walkingsound.png")
    assets.addWorldEvent("ACT_PUSHED", f"{worldevents}stumble.webp")
    assets.addWorldEvent("MVM_PUSH", f"{worldevents}pushing.png")
    assets.addWorldEvent("SND_PUSHED", f"{worldevents}stumble.webp")
    assets.addWorldEvent("MVM_GRAB", f"{worldevents}grabbingman.webp")
    assets.addWorldEvent("SND_MVM_GRAB", f"{worldevents}grabbingman.webp")
    assets.addWorldEvent("MVM_GRAB_HAND", f"{worldevents}grabhand.png")
    assets.addWorldEvent("MVM_GRABBED", f"{worldevents}beinggrabbed.png")
    assets.addWorldEvent("SND_GRABBED", f"{worldevents}beinggrabbed.png")
    assets.addWorldEvent("MVM_SILENT_WALK", f"{worldevents}stealthwalk.webp")
    assets.addWorldEvent("MVM_TRICKY_PUNCH", f"{worldevents}trickypunch.jpeg")
    assets.addWorldEvent("SND_MVM_TRICKY_PUNCH", f"{worldevents}punchmovement.png")
    assets.addWorldEvent("SND_TRICKY_PUNCH", f"{worldevents}trickypunch.jpeg")
    assets.addWorldEvent("SND_TRICKY_CRITIC", f"{worldevents}criticalpunch.jpg")

    # CHRST

    

    assets.addError(f"{path}images/bluescreen.webp")

    assets.addSound("error",f"{path}sounds/error.wav")
    assets.addSound("EventerRequest",f"{path}sounds/eventerrequest.wav")
    assets.addSound("OtherRequests",f"{path}sounds/otherrequests.wav")
    assets.addSound("CharacterSelection",f"{path}sounds/characterselection.wav")
    assets.addSound("EndTurn",f"{path}sounds/endturn.wav")
    assets.addSound("SND_WALK",f"{path}sounds/walk.wav")
    assets.addSound("SND_PUNCH",f"{path}sounds/punch.wav")
    return assets