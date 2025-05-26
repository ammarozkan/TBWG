import socket as THESOCKET
from tbwgcon1_essentials import *


tbwgname = "tbwg"
tbwgversion = (0,0,0)
TBWGCON1_STD_NAME_SIZE = 32

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
    if name == "CharacterInformation":
        code = int.from_bytes(data[0:4],"little")
        hp = getThingFromBytes(data[4:4+TBWGiValue_size], "iValue")
        e = getThingFromBytes(data[4+TBWGiValue_size:4+TBWGiValue_size*2], "iValue")
        se = getThingFromBytes(data[4+TBWGiValue_size*2:4+TBWGiValue_size*3], "iValue")
        return TBWGCharacterInformation(code, hp, e, se)
    elif name == "iValue":
        value = int.from_bytes(data[0:4],"little")
        max = int.from_bytes(data[4:8],"little")
        return TBWGiValue(value,max)

def getRestPkgFromSocket(socket, pkgcode, size):
    size -= TBWGHeader_size
    data = socket.recv(size)
    if pkgcode == TBWGEnteringResponse_code:
        errcode = int.from_bytes(data[0:4],"little")
        nextchapter = int.from_bytes(data[4:8],"little")
        return TBWGEnteringResponse(errcode, nextchapter)
    elif pkgcode == TBWGCharacterInformator_code:
        chrcount = int.from_bytes(data[0:4],"little")
        characterinfs_byte = []
        for i in range(0,chrcount):
            padd = TBWGCharacterInformation_size*i
            characterinfs_byte.append( data[4+padd:4+TBWGCharacterInformation_size+padd] )
        characters = [getThingFromBytes(c,"CharacterInformation") for c in characterinfs_byte]
        return TBWGCharacterInformator(characters)
    elif pkgcode == TBWGCharacterSelectionError_code:
        errcode = int.from_bytes(data[0:4],"little")
        nextchapter = int.from_bytes(data[4:8],"little")
        return TBWGCharacterSelectionError(errcode, nextchapter)



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
    print(r)
    return r

def getBytesFromCharacterSelection(chrsl):
    selection = chrsl.selection.to_bytes(1,"little")
    options = chrsl.options.to_bytes(1,"little")

    slbytes = selection+options

    return getAsBytes(getDefaultHeader(TBWGCharacterSelection_code, 12+len(slbytes)))+slbytes


def getAsBytes(pkg):
    if type(pkg) == TBWGHeader:
        return getBytesFromHeader(pkg)
    elif type(pkg) == TBWGEnteringPackage:
        return getBytesFromEnteringPackage(pkg)
    elif type(pkg) == TBWGCharacterSelection:
        return getBytesFromCharacterSelection(pkg)




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
        slc_chr = self.characterselector(chr_inf.characters)
        self.send(TBWGCharacterSelection(slc_chr, 0))

        slc_err = self.recv()
        if type(slc_err) != TBWGCharacterSelectionError: return 0
        print(slc_err)
        if slc_err.errcode != 0:
            return self.connect_chapter(slc_err.nextchapter)
        
        return 1
    


    def recv(self):
        header = getHeaderFromSocket(self.socket)
        if not TBWGHEADERCHECK(header):
            print("FAILURE ON HEADER CHECK")
            return None
        pkg = getRestPkgFromSocket(self.socket, header.pkgcode, header.size)
        return pkg
    def send(self, pkg):
        if type(pkg) == TBWGHeader:
            pkg.size = TBWGHeader_size
        data = getAsBytes(pkg)
        self.socket.send(data)