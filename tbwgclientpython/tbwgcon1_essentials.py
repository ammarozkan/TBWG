
REQ_POSITION = 0
REQ_AREA = 1
REQ_DIRECTION = 2


TBWGDigits_size = 4
class TBWGDigits:
    def __init__(self, uint):
        self.value = uint
    def __getitem__(self, i):
        return (self.value & (1 << i)) > 0
    def __str__(self):
        res = ""
        for i in range(0,32):
            c = "0"
            if (self.value&(1<<i)): c = "1"
            res += c
        return res

TBWGiValue_size = 8
class TBWGiValue:
    def __init__(self, value, max):
        self.value = value
        self.max = max
    def __str__(self):
        return f"[| {self.value}/{self.max} |]"

TBWGiVector_size = 8
class TBWGiVector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return f"[| ({self.x},{self.y}) |]"

TBWGfVector_size = 8
class TBWGfVector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return f"[| ({self.x},{self.y}) |]"


TBWGCharacterInformation_size = 32
class TBWGCharacterInformation:
    def __init__(self, ID, characterCode, position, direction, hp):
        self.ID = ID
        self.characterCode = characterCode
        self.position = position
        self.direction = direction
        self.hp = hp
    def __str__(self):
        return f"[| Character {self.characterCode} | HP:{self.hp} POS:{self.position} DIR:{self.direction} ID:{self.ID} |]"

TBWGEntityInformation_size = 6*4
class TBWGEntityInformation:
    def __init__(self, ID, entityCode, position, direction):
        self.ID = ID
        self.entityCode = entityCode
        self.position = position
        self.direction = direction
    def __str__(self):
        return f"[| Entity {self.entityCode} | POS:{self.position} DIR:{self.direction} ID:{self.ID} |]"

TBWGConCharacterInformation_size = 28
class TBWGConCharacterInformation:
    def __init__(self, code, hp, e, se):
        self.code = code
        self.hp = hp
        self.e = e
        self.se = se
    def __str__(self):
        return f"[| ConCharacter {self.code} | HP:{self.hp} E:{self.e} SE:{self.se} |]"

TBWGConEventerInformation_size = 76
class TBWGConEventerInformation:
    def __init__(self, eventerCode, ID, energy, spellenergy, eventertype, requiredinformations, costs, details):
        self.eventerCode = eventerCode
        self.ID = ID
        self.energy = energy
        self.spellenergy = spellenergy
        self.eventertype = eventertype
        self.requiredinformations = requiredinformations
        self.costs = costs
        self.details = details
    def __str__(self):
        return f"[| Eventer {self.eventerCode} | {self.energy}E {self.spellenergy}SE {self.costs} | {self.eventertype} typed | {self.requiredinformations} |]"


TBWGStats_size = 28
class TBWGStats:
    def __init__(self, STR, DEX, CNS, WIS, SCS, SPD, VISRES):
        self.STR = STR
        self.DEX = DEX
        self.CNS = CNS
        self.WIS = WIS
        self.SCS = SCS
        self.SPD = SPD
        self.VISRES = VISRES
    def __str__(self):
        return f"[| Stats | STR:{self.STR} |]"

TBWGConObservingEffectInformation_size = 8+32
class TBWGConObservingEffectInformation:
    def __init__(self, ID, code, details):
        self.ID = ID
        self.code = code
        self.details = details
    def __str__(self):
        return f"[| Effect {self.code} | ID:{self.ID} |]"

TBWGUsersEventerInformation_size = 45
class TBWGUsersEventerInformation:
    def __init__(self, eventerCode, ID, energyValueType, energy, spellEnergy, eventer_type, required_informations, eventeruses):
        self.eventerCode = eventerCode
        self.ID = ID
        self.energyValueType = energyValueType
        self.energy = energy
        self.spellEnergy = spellEnergy
        self.eventer_type = eventer_type
        self.required_informations = required_informations
        self.eventeruses = eventeruses
    def __str__(self):
        return f"[| Eventer {self.eventerCode} | ID:{self.ID} | code:{self.eventerCode} |]"

TBWGEventerUses_size = 20
class TBWGEventerUses:
    def __init__(self, classic, arm, hand, movement, thought):
        self.classic = classic
        self.arm = arm
        self.hand = hand
        self.movement = movement
        self.thought = thought
    def __str__(self):
        result = ""
        if self.classic > 0: result += f"{self.classic} classic "
        if self.arm > 0: result += f"{self.arm} arm "
        if self.hand > 0: result += f"{self.hand} hand "
        if self.movement > 0: result += f"{self.movement} movement "
        if self.thought > 0: result += f"{self.thought} thought "
        
        if result == "": return ""
        else: return result[:len(result)-1] + "."



EFFECT_TRIGGER_TYPE_COUNT = 5

TBWGHeader_size = 12
class TBWGHeader:
    def __init__(self, tbwgname,version,pkgcode):
        self.tbwgname = tbwgname
        self.version = version
        self.pkgcode = pkgcode
        self.size = 0
    def __str__(self):
        return f"[| Header | tbwgname:{self.tbwgname} | version:{self.version} | pkgcode:{self.pkgcode} | size:{self.size} |]"

TBWGEnteringPackage_code = 0
class TBWGEnteringPackage:
    def __init__(self, name):
        self.name = name
        self.namesize = len(name)+1
    def __str__(self):
        return f"[| EnterinPackage | name:{self.name} | namesize:{self.namesize} |]"

TBWGEnteringResponse_code = 1
class TBWGEnteringResponse:
    def __init__(self, errcode, nextchapter):
        self.errcode = errcode
        self.nextchapter = nextchapter
    def __str__(self):
        return f"[| EnteringResponse | errcode:{self.errcode} | nextchapter:{self.nextchapter} |]"

TBWGCharacterInformator_code = 4
class TBWGCharacterInformator:
    def __init__(self, characters):
        self.characters = characters
    def __str__(self):
        return f"[| CharacterInformator of {len(self.characters)} Character |]"

TBWGCharacterSelection_code = 5
class TBWGCharacterSelection:
    def __init__(self, selection, options):
        self.selection = selection
        self.options = options
    def __str__(self):
        return f"[| CharacterSelection | selection:{self.selection} | options:{self.options} |]"

TBWGCharacterSelectionError_code = 6
class TBWGCharacterSelectionError:
    def __init__(self, errcode, nextchapter):
        self.errcode = errcode
        self.nextchapter = nextchapter
    def __str__(self):
        return f"[| CharacterSelectionError | errcode:{self.errcode} | nextchapter:{self.nextchapter} |]"


TBWGObservingInformationHeader_code = 33
class TBWGObservingInformationHeader:
    def __init__(self, effectCounts, eventerCount, characterInformationCount, entityInformationCount):
        self.effectCounts = effectCounts
        self.eventerCount = eventerCount
        self.characterInformationCount = characterInformationCount
        self.entityInformationCount = entityInformationCount
    def __str__(self):
        return f"[| ObservingInformationHeader | {self.effectCounts} effects | {self.eventerCount} eventers | {self.characterInformationCount} characters | {self.entityInformationCount} entities |]"
    


TBWGObservingInformation_code = 34
TBWGObservingInformation_shape = "IiiiiiiiiiiiiiiiffI"
class TBWGObservingInformation:
    def __init__(self,selfid,stats,hp,e,se,position,direction,state,tool,effects,eventers,characters,entities):
        self.selfid = selfid
        self.stats = stats
        self.hp = hp
        self.e = e 
        self.se = se
        self.position = position
        self.direction = direction
        self.state = state
        self.tool = tool
        self.effects = effects
        self.eventers = eventers
        self.characters = characters
        self.entities = entities
    def __str__(self):
        return f"[| ObservingInformation for {self.selfid} |]"

TBWGWorldEventInformation_code = 35
class TBWGWorldEventInformation:
    def __init__(self, selfid, eventname, position):
        self.selfid = selfid
        self.eventname = eventname
        self.position = position
    def __str__(self):
        return f"[| WorldEvent {self.eventname} for {self.selfid} | POS:{self.position} |]"

TBWGEventerOptionsInformationHeader_code = 36
class TBWGEventerOptionsInformationHeader:
    def __init__(self, ID, allowedEventerTypes, eventeruses, eventercount):
        self.ID = ID
        self.allowedEventerTypes = allowedEventerTypes
        self.eventeruses = eventeruses
        self.eventercount = eventercount
    def __str__(self):
        return f"[| EventerOptionsInformationHeader {self.ID} | {self.allowedEventerTypes} types | {self.eventeruses} | {self.eventercount} eventers |]"


TBWGEventerOptionsInformation_code = 38
class TBWGEventerOptionsInformation:
    def __init__(self, eventers):
        self.eventers = eventers
    def __str__(self):
        return f"[| EventerOptionsInformation of {len(self.eventers)} eventers |]"


class TBWGEventerRequiredInformations:
    def __init__(self, position = TBWGiVector(0,0), position2 = TBWGiVector(0,0), direction = TBWGfVector(0,0), A = TBWGiVector(0,0), B = TBWGiVector(0,0)):
        self.position = position
        self.position2 = position2
        self.direction = direction
        self.A = A
        self.B = B


TURNPLAY_END_TURN = (1<<1)
TURNPLAY_CHOOSED_NONE = (1<<2)

TBWGConTurnPlay_code = 39
class TBWGConTurnPlay:
    def __init__(self, eventer_th, required_informations, specs):
        self.eventer_th = eventer_th
        self.required_informations = required_informations
        self.specs = specs
        pass
    def __str__(self):
        return f"[| TurnPlay | eventer_th:{self.eventer_th} | reqs:{self.required_informations} | specs:{self.specs} |]"