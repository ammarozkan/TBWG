import socket as THESOCKET
import struct
from tbwgcon1_essentials import *


tbwgname = "tbwg"
tbwgversion = (0,0,0)
TBWGCON1_STD_NAME_SIZE = 32
TBWGCON1_STD_EVENTNAME_SIZE = TBWGCON1_STD_NAME_SIZE

def TBWGHEADERCHECK(hdr):
    if hdr.tbwgname != tbwgname: 
        print(f"Name is {hdr.tbwgname} instead of {tbwgname}")
        return 0
    st = 1
    for i in range(0,3):
        if hdr.version[i] != tbwgversion[i]: st = 0
    return st

def getstdized(inp):
    if type(inp) == str:
        return getstdized((inp+"\0").encode('utf-8'))
    elif type(inp) == bytes:
        while len(inp) < TBWGCON1_STD_NAME_SIZE:
            inp += int(0).to_bytes(1,'little')
        return inp

def getHeaderFromSocket(socket):
    data = socket.recv(8+4)
    tbwgname = data[:4].decode("utf-8")
    version = tuple([i for i in data[4:7]])
    pkgcode = data[7]
    size = int.from_bytes(data[8:12], "little")

    tbwgheader = TBWGHeader(tbwgname,version,pkgcode)
    tbwgheader.size = size
    return tbwgheader

def getThingFromBytes(data, name):
    if name == "string":
        d = ""
        for di in data:
            if di == 0: break
            d+=chr(di)
        return d
    elif name == "uint32_t":
        return struct.unpack("I",data)[0]
    elif name == "int32_t":
        return struct.unpack("i",data)[0]
    elif name == "digits":
        return TBWGDigits(struct.unpack("I",data)[0])
    elif name == "ConCharacterInformation":
        cps = 0
        code = int.from_bytes(data[cps:cps+4],"little") ; cps += 4
        hp = getThingFromBytes(data[cps:cps+TBWGiValue_size], "iValue") ; cps += TBWGiValue_size
        e = getThingFromBytes(data[cps:cps+TBWGiValue_size], "iValue") ; cps += TBWGiValue_size
        se = getThingFromBytes(data[cps:cps+TBWGiValue_size], "iValue") ; cps += TBWGiValue_size
        return TBWGConCharacterInformation(code, hp, e, se)
    elif name == "iValue":
        b = struct.unpack("ii",data)
        return TBWGiValue(b[0],b[1])
    elif name == "iVector":
        b = struct.unpack("ii",data)
        return TBWGiVector(b[0],b[1])
    elif name == "fVector":
        x,y = struct.unpack("ff",data)
        return TBWGfVector(x,y)
    elif name == "EventerUses":
        values = struct.unpack("iiiii",data)
        return TBWGEventerUses(*values)
    elif name == "Stats":
        values = [int.from_bytes(data[i*4:(i+1)*4], "little") for i in range(0,7)]
        return TBWGStats(*values)
    elif name == "ObservingEffectInformation":
        ID, code = struct.unpack("II", data)
        return TBWGConObservingEffectInformation(ID, code)
    elif name == "UsersEventerInformation":
        eventerCode, ID = struct.unpack("II", data[:8])
        energyValueType = data[8]
        energy, spellEnergy, eventer_type, required_informations = struct.unpack("IIII",data[9:25])
        name = getThingFromBytes(data[25:57], "string")
        #eventerCode, ID, energyValueType, energy, spellEnergy, eventer_type, required_informations = b[:7]
        eventeruses = getThingFromBytes(data[57:77],"EventerUses")
        print(data[77:])
        return TBWGUsersEventerInformation(eventerCode, ID, energyValueType, energy, spellEnergy, eventer_type, required_informations, name, eventeruses)
    elif name == "CharacterInformation":
        b = list(struct.unpack("IIiiffii",data))
        ID, characterCode = b[0], b[1]
        position = TBWGiVector(b[2],b[3])
        direction = TBWGfVector(b[4],b[5])
        hp = TBWGiValue(b[6],b[7])
        print(position)
        return TBWGCharacterInformation(ID, characterCode, position, direction, hp)
    elif name == "EntityInformation":
        b = list(struct.unpack("IIiiff"))
        ID, entityCode = b[0], b[1]
        position = TBWGiVector(b[2], b[3])
        direction = TBWGfVector(b[4], b[5])
        return TBWGEntityInformation(ID, entityCode, position, direction)
    elif name == "EventerInformation":
        b = data
        eventerCode = getThingFromBytes(b[0:4],"uint32_t")
        ID = getThingFromBytes(b[4:8],"uint32_t")
        energy = getThingFromBytes(b[8:12],"uint32_t")
        spellenergy = getThingFromBytes(b[12:16],"uint32_t")
        eventertype = getThingFromBytes(b[16:20],"digits")
        requiredinformations = getThingFromBytes(b[20:24],"digits")
        name = getThingFromBytes(b[24:56],"string")
        costs = getThingFromBytes(b[56:76],"EventerUses")

        return TBWGConEventerInformation(eventerCode, ID, energy, spellenergy, eventertype, requiredinformations, name, costs)
    else:
        raise Exception(f"YOOOOOOO WTH IS {name}!!")


def getRestPkgFromSocket(socket, pkgcode, size, extraheader = None):
    size -= TBWGHeader_size
    data = socket.recv(size)
    if pkgcode == TBWGEnteringResponse_code:
        errcode, nextchapter = struct.unpack("II",data)
        return TBWGEnteringResponse(errcode, nextchapter)
    elif pkgcode == TBWGCharacterInformator_code:
        p = data[0:2]+bytes(2)
        chrcount = struct.unpack("I",p[0:4])[0]
        characterinfs_byte = []
        cps = 2
        for i in range(0,chrcount):
            padd = TBWGConCharacterInformation_size
            characterinfs_byte.append( data[cps:cps+padd] )
            cps+=padd
            print(data[cps:cps+padd])
        characters = [getThingFromBytes(c,"ConCharacterInformation") for c in characterinfs_byte]
        return TBWGCharacterInformator(characters)
    elif pkgcode == TBWGCharacterSelectionError_code:
        errcode, nextchapter = struct.unpack("II",data)
        return TBWGCharacterSelectionError(errcode, nextchapter)
    elif pkgcode == TBWGObservingInformationHeader_code:
        values = []
        for i in range(0,EFFECT_TRIGGER_TYPE_COUNT + 3):
            values.append( getThingFromBytes(data[i*4:(i+1)*4], "uint32_t") )
        effectCounts = values[:EFFECT_TRIGGER_TYPE_COUNT]
        eventerCount = values[EFFECT_TRIGGER_TYPE_COUNT]
        characterInformationCount = values[EFFECT_TRIGGER_TYPE_COUNT+1]
        entityInformationCount = values[EFFECT_TRIGGER_TYPE_COUNT+2]
        return TBWGObservingInformationHeader(effectCounts, eventerCount, characterInformationCount, entityInformationCount)
    elif pkgcode == TBWGObservingInformation_code:
        if type(extraheader) != TBWGObservingInformationHeader: return None

        selfid = getThingFromBytes(data[0:4],"uint32_t")
        stats = getThingFromBytes(data[4:32],"Stats")
        hp = getThingFromBytes(data[32:40],"iValue")
        e = getThingFromBytes(data[40:48],"iValue")
        se = getThingFromBytes(data[48:56],"iValue")
        position = getThingFromBytes(data[56:64],"iVector")
        direction = getThingFromBytes(data[64:72],"fVector")
        state = getThingFromBytes(data[72:76],"uint32_t")

        print("extraheader:",extraheader)
        lastp = 76
        effects = []
        
        effectcount = sum(extraheader.effectCounts)
        effects = []
        for i in range(0, effectcount):
            padd = TBWGConObservingEffectInformation_size
            effectinformationbytes = data[lastp:lastp+padd]
            lastp += padd

            effectinformation = getThingFromBytes(effectinformationbytes, "ObservingEffectInformation")
            effects.append(effectinformation)
        

        eventercount = extraheader.eventerCount
        eventers = []
        for i in range(0, eventercount):
            padd = TBWGUsersEventerInformation_size
            eventerinformationbytes = data[lastp:lastp+padd]
            lastp += padd

            eventerinformation = getThingFromBytes(eventerinformationbytes, "UsersEventerInformation")
            eventers.append(eventerinformation)
            print(eventerinformation)

        print(lastp)

        charactercount = extraheader.characterInformationCount
        characters = []
        for i in range(0,charactercount):
            padd = TBWGCharacterInformation_size
            characterinformationbytes = data[lastp:lastp+padd]
            lastp += padd

            characterinformation = getThingFromBytes(characterinformationbytes, "CharacterInformation")
            characters.append(characterinformation)

        entitycount = extraheader.entityInformationCount
        entities = []
        
        for i in range(0,entitycount):
            padd = TBWGEntityInformation_size
            entityinformationbytes = data[lastp:lastp+padd]
            lastp += padd

            entity = getThingFromBytes(entityinformationbytes, "EntityInformation")
            entities.append( entity )
        
        if lastp > len(data): raise Exception("Request of reading more than the data. Dude wth?")

        if lastp != len(data): raise Exception(f"DATA IS NOT FULLY READEN BY MISTAKE {len(data)-lastp} to go")

        return TBWGObservingInformation(selfid, stats, hp, e, se, position, direction, state, effects, eventers, characters, entities)
    elif pkgcode == TBWGWorldEventInformation_code:
        ID = getThingFromBytes(data[0:4], "uint32_t")
        name = getThingFromBytes(data[4:36],"string")
        iVector = getThingFromBytes(data[36:44], "iVector")

        return TBWGWorldEventInformation(ID,name,iVector)
    elif pkgcode == TBWGEventerOptionsInformationHeader_code:
        ID = getThingFromBytes(data[0:4], "uint32_t")
        allowedEventerTypes = getThingFromBytes(data[4:8], "uint32_t")
        eventeruses = getThingFromBytes(data[8:28], "EventerUses")
        eventercount = getThingFromBytes(data[28:32], "uint32_t")
        return TBWGEventerOptionsInformationHeader(ID, allowedEventerTypes, eventeruses, eventercount)
    elif pkgcode == TBWGEventerOptionsInformation_code:
        eventercount = extraheader.eventercount

        eventerinfos = []
        lastp = 0
        for i in range(0, eventercount):
            tsize = TBWGConEventerInformation_size
            eventerinformationbytes = data[lastp:lastp+tsize]
            print("EVENTIERIOG:",len(eventerinformationbytes))

            eventerinformation = getThingFromBytes(eventerinformationbytes,"EventerInformation")
            eventerinfos.append(eventerinformation)
            
            lastp += tsize
        return TBWGEventerOptionsInformation(eventerinfos)
    else:
        raise Exception(f"Unknown PKGCODE : {pkgcode}")







def getDefaultHeader(pkgcode, size):
    h = TBWGHeader(tbwgname, tbwgversion, pkgcode)
    h.size = size
    return h

def getBytesFromHeader(tbwgheader):
    tbwgname = tbwgheader.tbwgname.encode("utf-8")[:4]
    version = tbwgheader.version[0].to_bytes(1, 'little')
    for i in range(1,3):
        version += tbwgheader.version[i].to_bytes(1, 'little')
    pkgcode = tbwgheader.pkgcode.to_bytes(1, "little")
    size = tbwgheader.size.to_bytes(4, "little")
    return tbwgname+version+pkgcode+size

def getBytesFromEnteringPackage(enteringpackage):
    name = getstdized(enteringpackage.name)
    namesize = enteringpackage.namesize.to_bytes(4,"little")
    enteringbytes = namesize+name

    r = getAsBytes(getDefaultHeader(TBWGEnteringPackage_code, 12+len(enteringbytes)))+enteringbytes
    return r

def getBytesFromCharacterSelection(chrsl):
    selection = chrsl.selection.to_bytes(1,"little")
    options = chrsl.options.to_bytes(1,"little")

    slbytes = selection+options

    return getAsBytes(getDefaultHeader(TBWGCharacterSelection_code, 12+len(slbytes)))+slbytes

def getBytesFromTurnPlay(evnsl):
    eventer_th = evnsl.eventer_th.to_bytes(4,"little")
    requiredInformations = getAsBytes(evnsl.required_informations)
    specs = evnsl.specs.to_bytes(4,"little")
    
    slbytes = eventer_th+requiredInformations+specs
    return getAsBytes(getDefaultHeader(TBWGConTurnPlay_code, 12+len(slbytes)))+slbytes

def getBytesFromRequiredInformations(reqs):
    result = getAsBytes(reqs.position)
    result += getAsBytes(reqs.position2)
    result += getAsBytes(reqs.direction)
    result += getAsBytes(reqs.A)
    result += getAsBytes(reqs.B)
    return result

def getAsBytes(pkg):
    if type(pkg) == TBWGHeader:
        return getBytesFromHeader(pkg)
    elif type(pkg) == TBWGEnteringPackage:
        return getBytesFromEnteringPackage(pkg)
    elif type(pkg) == TBWGCharacterSelection:
        return getBytesFromCharacterSelection(pkg)
    elif type(pkg) == TBWGConTurnPlay:
        return getBytesFromTurnPlay(pkg)
    elif type(pkg) == TBWGEventerRequiredInformations:
        return getBytesFromRequiredInformations(pkg)
    elif type(pkg) == TBWGiVector:
        return struct.pack("ii",pkg.x,pkg.y)
    elif type(pkg) == TBWGfVector:
        return struct.pack("ff",pkg.x,pkg.y)
    elif type(pkg) == TBWGiValue:
        return struct.pack("ii",pkg.value,pkg.max)




### CLIENT

class TBWGConClient:
    def __init__(self, ip : str, port : int):
        self.socket = THESOCKET.socket(THESOCKET.AF_INET, THESOCKET.SOCK_STREAM)
        self.socket.connect((ip, port))

        self.connect_chapters = [self.connect_chapter1,self.connect_chapter2]
    def setname(self,name):
        self.name = name
    def setcharacterselector(self, selector):
        self.characterselector = selector
    
    def connect(self):
        return self.connect_chapter1()
    
    def connect_chapter(self, chp : int):
        print(f"Skipping to {chp}")
        return self.connect_chapters[chp-1]()
    
    def connect_chapter1(self):
        print("Chapter1")
        entering = TBWGEnteringPackage(self.name)
        self.send(entering)
        e = self.recv()
        if type(e) != TBWGEnteringResponse: return 0
        if e.errcode == 0:
            return self.connect_chapter(e.nextchapter)
        else:
            print(f"ERR {e.errcode} on CHAPTER1")
            return 0
    
    def connect_chapter2(self):
        print("Chapter2")
        chr_inf = self.recv()
        if type(chr_inf) != TBWGCharacterInformator: return 0
        for c in chr_inf.characters:
            print(f"\t\t{c}")
        slc_chr = self.characterselector(chr_inf.characters)
        self.send(TBWGCharacterSelection(slc_chr, 0))

        slc_err = self.recv()
        if type(slc_err) != TBWGCharacterSelectionError: return 0
        print(slc_err)
        if slc_err.errcode != 0:
            return self.connect_chapter(slc_err.nextchapter)
        
        return 1

    def recv(self, extraheader = None):
        header = getHeaderFromSocket(self.socket)
        if not TBWGHEADERCHECK(header):
            raise Exception("FAILURE ON HEADER CHECK")
        pkg = getRestPkgFromSocket(self.socket, header.pkgcode, header.size, extraheader = extraheader)
        print("RECVPKG:",pkg)
        return pkg
    def send(self, pkg):
        if type(pkg) == TBWGHeader:
            pkg.size = TBWGHeader_size
        print("SENDPKG:",pkg)
        data = getAsBytes(pkg)
        self.socket.send(data)

    

    
    def step(self, observe, worldevent, eventerselector):
        pkg = self.recv()
        if type(pkg) == TBWGObservingInformationHeader:
            pkg2 = self.recv(extraheader = pkg)
            observe(pkg,pkg2)
        elif type(pkg) == TBWGWorldEventInformation:
            worldevent(pkg)
        elif type(pkg) == TBWGEventerOptionsInformationHeader:
            pkg2 = self.recv(extraheader = pkg)
            turnplay = eventerselector(pkg,pkg2)
            self.send(turnplay)
        return True


def getClientByAdress(addr):
    print("ADRRRR:",addr)
    addr,port = addr.split(":")
    ip = THESOCKET.gethostbyname(addr)

    return TBWGConClient(ip,int(port))