
TBWGHeader_size = 12
class TBWGHeader:
    def __init__(self, tbwgname,version,pkgcode):
        self.tbwgname = tbwgname
        self.version = version
        self.pkgcode = pkgcode
        self.size = 0
    def __str__(self):
        return f"Header | tbwgname:{self.tbwgname} | version:{self.version} | pkgcode:{self.pkgcode} | size:{self.size}"

TBWGEnteringPackage_code = 0
class TBWGEnteringPackage:
    def __init__(self, name):
        self.name = name
        self.namesize = len(name)+1
    def __str__(self):
        return f"EnterinPackage | name:{self.name} | namesize:{self.namesize}"

TBWGEnteringResponse_code = 1
class TBWGEnteringResponse:
    def __init__(self, errcode, nextchapter):
        self.errcode = errcode
        self.nextchapter = nextchapter
    def __str__(self):
        return f"EnteringResponse | errcode:{self.errcode} | nextchapter:{self.nextchapter}"

TBWGCharacterInformator_code = 4
class TBWGCharacterInformator:
    def __init__(self, characters):
        self.characters = characters

TBWGCharacterSelection_code = 5
class TBWGCharacterSelection:
    def __init__(self, selection, options):
        self.selection = selection
        self.options = options

TBWGCharacterSelectionError_code = 6
class TBWGCharacterSelectionError:
    def __init__(self, errcode, nextchapter):
        self.errcode = errcode
        self.nextchapter = nextchapter
    def __str__(self):
        return f"CharacterSelectionError | errcode:{self.errcode} | nextchapter:{self.nextchapter}"


TBWGiValue_size = 8
class TBWGiValue:
    def __init__(self, value, max):
        self.value = value
        self.max = max
    def __str__(self):
        return f"[|{self.value}/{self.max}|]"

TBWGCharacterInformation_size = 4 + 3*TBWGiValue_size
class TBWGCharacterInformation:
    def __init__(self, code, hp, e, se):
        self.code = code
        self.hp = hp
        self.e = e
        self.se = se
    def __str__(self):
        return f"Character {self.code} | HP:{self.hp} E:{self.e} SE:{self.se}"