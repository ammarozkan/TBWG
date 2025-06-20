import tbwgplayer as Player
import tbwgcon1

def program(assets = Player.Assets(), tbwgconenable = True, ip="127.0.0.1", port=5005, name="John"):

    Player.init()
    player = Player.TBWGPyGamePlayer(1280, 720, assets=assets)
    #player.characters = [tbwgcon1.TBWGCharacterInformation(6, 32, tbwgcon1.TBWGiVector(0,2), tbwgcon1.TBWGfVector(0.0,1.0), tbwgcon1.TBWGiValue(6,10))]


    def observe(obshdr : tbwgcon1.TBWGObservingInformationHeader, observation : tbwgcon1.TBWGObservingInformation):
        for c in observation.characters:
            print(c)
        player.characters = observation.characters
        player.observer = (observation.position, observation.hp, observation.direction)
        
        player.energy = (observation.e.value, observation.e.max)
        player.spellenergy = (observation.se.value, observation.se.max)

        player.oneffects = []
        for ef in observation.effects:
            player.oneffects.append(str(ef.code))

    def worldevent(wevnt : tbwgcon1.TBWGWorldEventInformation):
        player.worldevents.append((wevnt.eventname,(wevnt.position.x,wevnt.position.y)))

    def eventer(evntrhdr : tbwgcon1.TBWGEventerOptionsInformationHeader, eventeroptinfos : tbwgcon1.TBWGEventerOptionsInformation):
        player.setRestUses([evntrhdr.eventeruses.classic,evntrhdr.eventeruses.fastcombat,evntrhdr.eventeruses.movement,evntrhdr.eventeruses.fastmagic,evntrhdr.eventeruses.thoughtmagic])

        for evn in eventeroptinfos.eventers:
            print(evn)

        eventer_th = player.request(Player.REQ_EVENTER, eventeroptinfos.eventers)
        evn = eventeroptinfos.eventers[eventer_th]

        reqinfs = tbwgcon1.TBWGEventerRequiredInformations()

        specs = 0
        if eventer_th == None or eventer_th == -1:
            specs = tbwgcon1.TURNPLAY_END_TURN
            return tbwgcon1.TBWGConTurnPlay(0, reqinfs, specs)

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
    assets = Player.Assets()
    assets.addCharacter(32,f"{path}/beg.jpg")
    assets.loadAll()


if __name__ == "__main__":
    assets = Player.Assets()
    assets.addBackground("default","images/frutigeraerobckr.webp")
    assets.addCharacter(0, "images/human2.png")
    assets.addCharacter(2, "images/fighter.png")
    assets.addCharacter(123, "images/avatar.png")
    assets.addEventer("TGF_PUNCH", "images/punch.jpg")
    assets.addEventer("TGF_LOOK", "images/look.png")
    assets.addEventer("TGF_WALK", "images/walkingman.jpg")
    assets.addEventer("TGF_REST", "images/resting.jpg")
    assets.addEventer("TGF_STFGHT_PUSH","images/push.png")
    assets.addEffect(16, "images/puzzled.png")
    assets.add("CancelButton", "images/please.gif")
    assets.add("DefaultButton","images/salutman.webp")
    assets.add("DefaultRestUses","images/token.png")
    assets.add("ClearWorldEvents","images/clear.png")
    assets.add("HealthIcon","images/heart.png")
    assets.addError("images/bluescreen.webp")
    assets.loadAll()

    program(assets=assets, tbwgconenable=True, ip="192.168.1.36", port=5005, name="John")