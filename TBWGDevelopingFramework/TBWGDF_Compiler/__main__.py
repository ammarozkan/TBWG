import CTokens, CPreProcessing, CWriter
import os

result = ""

effects = []
characters = []
eventers = []
entities = []
globalFunctions = []
PP = CPreProcessing.PreProcessor()

assetspath = ""

defaultCharacterIconPath = "default.webp"
defaultEntityIconPath = "default.webp"
defaultEventerIconPath = "default.webp"
defaultEffectIconPath = "default.webp"

def getAssetsPath(path):
    if not os.path.isfile(f"{assetspath}/{path}"):
        print(f"{path} is not a good asset path.")
        raise("path not cool.")
    return f"assets/{path}"

def getPythonViewFunctionCall(targetType, code, explanation, iconpath):
    explambdafnc = f"lambda source: f\"{explanation}\""
    return f"viewManager.Push{targetType.capitalize()}View({code},{explambdafnc},\"{getAssetsPath(iconpath)}\")\n"

def findDefByName(name):
    for g in effects+characters+globalFunctions+eventers+entities:
        if g.name == name: return g
    return None
    for e in effects:
        if e.name == name: return e
    for c in characters:
        if c.name == name: return c
    for f in globalFunctions:
        if f.name == name: return f
    for e in eventers:
        if e.name == name: return e

def InsideAtSigns(retriever : CTokens.Retriever):
    if retriever.get(1).text != "@": return

    result = ""

    elm = retriever.get(0).text
    target = retriever.get(2).text
    _target = findDefByName(target)
    if _target == None: 
        print(f"NULL TARGET AS SEARCHING '{target}'")
        raise("bop")
    if type(_target) == Effect and elm == "Get": result = f"{target}_GetEffect_base"
    elif type(_target) == Character and elm == "Get": result = f"{target}_GetCharacter_base"
    elif type(_target) == Entity and elm == "Get": result = f"{target}_GetEntity_base"
    elif elm == "Code": result = f"{_target.codename}"
    else: result = f"{target}_{elm}"

    retriever.modify(0, CTokens.Token(CTokens.NOTHING))
    retriever.modify(1, CTokens.Token(CTokens.NOTHING))
    retriever.modify(2, CTokens.Token(result))
    retriever.makeNothingsDisappear()
        

def ProcessInsideConversion(tokens):
    retriever = CTokens.Retriever(tokens)
    while retriever.next(usepasser = True):
        if retriever.check([CTokens.Token(CTokens.GREATTEXT),CTokens.Token("@"),CTokens.Token(CTokens.GREATTEXT)]):
            InsideAtSigns(retriever)
    return retriever.tokens

class Function:
    def __init__(self, type, name, parameters, inside, preparameters : dict):
        self.type = type
        self.name = name
        self.parameters = parameters
        self.inside = inside
        self.preparameters = preparameters
    def getHeader(self):
        ftkn = self.type+[self.name]+[CTokens.Token("(")]+self.parameters+[CTokens.Token(")")]+[CTokens.Token(";")]
        return CTokens.Retriever(ftkn).VomitAsText()
    def getSource(self):
        ftkn = self.type+[self.name]+[CTokens.Token("(")]+self.parameters+[CTokens.Token(")")]
        ftkn += [CTokens.Token("{")]+ProcessInsideConversion(self.inside)+[CTokens.Token("}")]
        return CTokens.Retriever(ftkn).VomitAsText()

class StolenFunction:
    def __init__(self, name):
        self.name = name

class Eventer:
    def __init__(self, name : str, inside):
        self.name = name
        self.inside = inside
        self.funcs = {}
        self.codename = f"EVENTER_CODENAME_{name.upper()}"
        self.code = 0

        self.changes = {}
    
    def getChange(self, change, default):
        if change in self.changes: return self.changes[change]
        else: return default
    def getConstructorName(self):
        if "Constructor" in self.funcs: return self.funcs["Constructor"].name.text
        else: return None
    def getGetEventerBase(self):
        return f"{self.name}_GetEventer_base"
    
    def addFunction(self, name, func : Function):
        self.funcs[name] = func
        self.funcs[name].name.text = f"{self.name}_{name}"
        fn = self.funcs[name]
        if name == "Constructor":
            fetcherParam = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}")
            if len(self.funcs[name].parameters) > 0: fetcherParam+=[CTokens.Token(",")]
            self.funcs[name].parameters = fetcherParam+fn.parameters
        elif name in ["Executer","CanExecutedNow"]:
            converter = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}=(struct {self.name}*)eventer;")
            self.funcs[name].inside = converter+fn.inside
            self.funcs[name].parameters = CTokens.QuickTokenize(f"void* eventer,")+self.funcs[name].parameters
        else:
            converter = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}=(struct {self.name}*)eventer;")
            addition = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}" + ("," if len(fn.parameters) != 0 else ""))
            self.funcs[name].parameters = addition+self.funcs[name].parameters

    def getRequiredInformations(self):
        result = "0|"
        parsedRequire = []
        if "require" in self.changes: parsedRequire = self.changes["require"].split(",")
        
        if "position" in parsedRequire: result += "EVENTER_REQUIRED_INFORMATION_POSITION|"
        if "direction" in parsedRequire: result += "EVENTER_REQUIRED_INFORMATION_DIRECTION|"

        return result[:len(result)-1]
    def getSource(self):


        result = f"struct Eventer* {self.name}_GetEventer_base()"
        result += "{"
        result += f"struct Eventer* eventer = GET_SIZED_STRUCT({self.name});"
        result += f"eventer->ID = getID();"
        result += f"eventer->eventerCode = {self.codename};"
        result += f"eventer->baseEnergy = {self.getChange("baseEnergy", "0")};"
        result += f"eventer->baseSpellEnergy = {self.getChange("baseSpellEnergy","0")};"

        result += f"eventer->eventer_type = {self.getChange("eventerType", "EVENTER_TYPE_CLASSIC")};"

        result += f"eventer->required_informations = {self.getRequiredInformations()};"

        values = ""
        for costName in ["classic","armMove", "handMove", "movement", "thought"]:
            values += self.getChange(costName, "0") + ","

        result += "struct EventerUses costs = { " + values[:len(values)-1] + " };"
        result += "eventer->costs = costs;"

        if "SetReady" in self.funcs: result += f"eventer->setReady = {self.funcs["SetReady"].name.text};"
        else: result += "eventer->setReady = defaultSetEventerReady;"

        if "Executer" in self.funcs: result += f"eventer->executer = {self.funcs["Executer"].name.text};"
        else: result += "eventer->executer = defaultEventerExecuter;"

        if "NotChoosed" in self.funcs: result += f"eventer->notChoosed = {self.funcs["NotChoosed"].name.text};"
        else: result += "eventer->notChoosed = defaultEventerNotChoosed;"

        if "CanExecutedNow" in self.funcs: result += f"eventer->canExecutedNow = {self.funcs["CanExecutedNow"].name.text};"
        else: result += "eventer->canExecutedNow = defaultEventerCanExecutedNow;"

        if "Constructor" in self.funcs:
            result += f"{self.getConstructorName()}((struct {self.name}*)eventer);"
        
        result += "for (unsigned int i = 0 ; i < 8 ; i += 1) eventer->details[i] = 0;"

        result += "return eventer;"
        result += "}"

        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getSource()
        return result
    def getHeader(self):
        result = ""
        
        result += f"struct {self.name}" + "{struct Eventer eventer;" +  f"{self.inside}" + "};"
        result += f"struct Eventer* {self.name}_GetEventer_base();"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getHeader()
        
        return result
    
    def changeInvoke(self, name, value):
        self.changes[name] = value
    
    

    def getViewerPy(self,lang=""):
        explanation = self.getChange(f"explanation{lang}", self.getChange("explanation", "No explanation entered."))
        iconPath = self.getChange(f"icon{lang}", self.getChange(f"icon", defaultEventerIconPath))
        
        return getPythonViewFunctionCall("Eventer", self.code, explanation, iconPath)    


class Character:
    def __init__(self, name : str, inside):
        self.name = name
        self.inside = inside
        self.funcs = {}
        self.codename = f"CHARACTER_CODENAME_{name.upper()}"
        self.code = 0

        self.changes = {}

        self.eventers = [] # compilation
        self.usingEventersOf = [] # character's name of eventers
        self.eventerSources = [] # for pre compilation
        self.eventersMerged = False
    
    def addEventer(self, eventer):
        self.eventers.append(eventer)
    
    def addUsingEventersOf(self, characterName):
        self.usingEventersOf.append(characterName)
    
    def addEventerSource(self, sourceName):
        self.eventerSources.append(sourceName)
    
    def resolveEventerSources(self):
        for s in self.eventerSources:
            df = findDefByName(s)
            if type(df) == Character:
                self.usingEventersOf.append(df)
            elif type(df) == Eventer:
                self.eventers.append(df)
            else:
                print(f"eventer source {s} on {self.name} is {df}, so finding function didnt returned a proper stuff.")
                raise("balloons")
        self.eventerSources = []
    
    def mergeEventersInOnePlace(self): # only after we're sure that every character is defined
        self.resolveEventerSources()
        if self.eventersMerged: return True
        for c in self.usingEventersOf:
            if type(c) != Character: continue
            if c.mergeEventersInOnePlace(): self.eventers = c.eventers + self.eventers
            else: return False
        
        self.eventersMerged = True
        return True
    
    def getChange(self, change, default):
        if change in self.changes: return self.changes[change]
        else: return default
    
    def addFunction(self, name, func : Function):
        self.funcs[name] = func
        self.funcs[name].name.text = f"{self.name}_{name}"
        fn = self.funcs[name]
        if name == "Constructor":
            fetcherParam = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}")
            if len(self.funcs[name].parameters) > 0: fetcherParam+=[CTokens.Token(",")]
            self.funcs[name].parameters = fetcherParam+fn.parameters
    
    def getConstructorModifications(self):
        ConstructorUse = ""
        params = []
        paramnamesuse = []
        if "Constructor" in self.funcs:
            params = self.funcs["Constructor"].parameters[4:]
            for j in range(1, len(params)):
                if j == len(params)-1 or params[j+1].type == CTokens.COMMA: paramnamesuse += [CTokens.Token(",")]+[params[j]]
            use = CTokens.QuickTokenize(f"(struct {self.name}*)character")+paramnamesuse
            ConstructorUse += f"{self.name}_Constructor({CTokens.Retriever(use).VomitAsText()});"
        params = CTokens.QuickTokenize("struct Dimension*dimension,iVector position")+params
        return params, ConstructorUse
    
    def getHeader(self):
        result = ""
        result += f"struct {self.name}" + "{struct Character character;" + f"{self.inside}" + "};"

        params, _ = self.getConstructorModifications()

        result += f"struct Character* {self.name}_GetCharacter_base({CTokens.Retriever(params).VomitAsText()});"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getHeader()
        return result
    
    def getSource(self):

        params, constrUse = self.getConstructorModifications()
        result = ""
        result += f"struct Character* {self.name}_GetCharacter_base({CTokens.Retriever(params).VomitAsText()})"
        result += "{"
        result += "struct Character* character = GET_SIZED_STRUCT(Character);"
        result += "character->b.tbwgType = TBWG_TYPE_CHARACTER;"
        result += f"character->b.code = {self.codename};"
        result += f"character->b.ID = getID();"
        result += f"character->b.position=position;"
        result += f"character->b.direction = getfVector(1.0f, 0.0f);"
        result += f"character->b.dimension = dimension;"
        result += f"character->b.mass=1.0f;"
        result += f"character->b.collisionFunction = beingDefaultOneWayCollision;"
        result += f"character->seeingResources = createList();"
        statNames = ["STR", "DEX", "CNS", "WIS", "SCS", "SPD","VISRES"]
        values = ""
        for k in ["STR", "DEX", "CNS", "WIS", "SCS", "SPD","VISRES"]:
            if k in self.changes: values += f"{self.changes[k]},"
            else: values += f"10,"
        values = values[:len(values)-1]

        result += "struct Stats s={" + values + "};"
        result += f"character->baseStats = s;"
        result += "character->stats = character->baseStats;"

        result += "character->b.visionHardness = 0;"
        values = "";i = 0;defaultEyes = [1.8, 1.0, 0.1, 1.0, 0.1]
        for k in ["eyeAngle","eyeLevel","eyeSpeed","hearingLevel","hearingSpeed"]:
            if k in self.changes: values += f"{CWriter.getAsCFloat(self.changes[k])},"
            else: values += f"{CWriter.getAsCFloat(defaultEyes[i])},"
            i+=1
        result += "struct Eye eye = { " + values + " };"
        result += "character->b.baseEye = eye;"
        result += "character->b.eye = eye;"

        result += "iValue hp = {" + self.getChange("health","20,20") + "};"
        result += "iValue e = {" + self.getChange("energy","40,50") + "};"
        result += "iValue se = {" + self.getChange("spellEnergy","0,0") + "};"

        result += "character->hp = hp;character->e=e;character->se=se;"

        result += "character->state = 0;"

        result += "character->passivePowerCount = 0;"
        result += "character->passivePowers = malloc(0);"

        result += "struct EventerUses newUses = {0,0,0,0,0};"
        result += "character->eventerSpendings = newUses;"
        
        eventerCount = len(self.eventers)
        result += f"character->eventerCount = {eventerCount};"
        result += f"character->eventers = malloc({eventerCount}*sizeof(struct Eventer*));"
        for i in range(eventerCount):
            result += f"character->eventers[{i}] = {self.eventers[i].getGetEventerBase()}();"
        
        result += f"for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1)"
        result += "{character->b.effects[i]=createList();}"

        result += "character->b.baseQueue=createQueue();"

        # Queue

        result += "struct QueueCharacterTurn* defaultCharTurn = malloc(sizeof(struct QueueCharacterTurn));"
        result += "(*defaultCharTurn)=getBasicCharacterTurn();"
        result += "defaultCharTurn->character = character;"
        result += "queueAddTurn(&(character->b.baseQueue), (struct QueueElementHeader*)defaultCharTurn);"

        result += "character->headHit = character->bodyHit = character->armHit = character->legHit = characterDefaultHit;"
        result += "character->energyRegener = characterDefaultEnergyRegener;"
        result += "character->healthRegener = characterDefaultHealthRegener;"

        result += "character->controllerInterface = getDefaultControllerInterface();"

        result += "character->seeCharacter = defaultSeeCharacter;"
        result += "character->b.canSeen = defaultCanSeen;"
        result += "character->seeWorldEvent = defaultSeeWorldEvent;"

        if "Constructor" in self.funcs: result += constrUse

        result += "return character;"


        result += "}"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getSource()
        return result

    def getViewerPy(self,lang=""):
        explanation = self.getChange(f"explanation{lang}", self.getChange("explanation", "No explanation entered."))
        iconPath = self.getChange(f"icon{lang}", self.getChange(f"icon", defaultCharacterIconPath))

        return getPythonViewFunctionCall("Character", self.code, explanation, iconPath)    
    
    def changeInvoke(self, name, value):
        self.changes[name] = value

class Entity:
    def __init__(self, name : str, inside):
        self.name = name
        self.inside = inside
        self.funcs = {}
        self.codename = f"ENTITY_CODENAME_{name.upper()}"
        self.code = 0

        self.changes = {}
    
    def getChange(self, change, default):
        if change in self.changes: return self.changes[change]
        else: return default
    
    def addFunction(self, name, func : Function):
        self.funcs[name] = func
        self.funcs[name].name.text = f"{self.name}_{name}"
        fn = self.funcs[name]
        if name == "Constructor":
            fetcherParam = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}")
            if len(self.funcs[name].parameters) > 0: fetcherParam+=[CTokens.Token(",")]
            self.funcs[name].parameters = fetcherParam+fn.parameters
        elif name in ["Eventer"]:
            converter = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}=(struct {self.name}*)entity;")
            self.funcs[name].inside = converter+fn.inside
            self.funcs[name].parameters = CTokens.QuickTokenize(f"void* entity,")+self.funcs[name].parameters
    
    def getConstructorModifications(self):
        ConstructorUse = ""
        params = []
        paramnamesuse = []
        if "Constructor" in self.funcs:
            params = self.funcs["Constructor"].parameters[4:]
            for j in range(1, len(params)):
                if j == len(params)-1 or params[j+1].type == CTokens.COMMA: paramnamesuse += [CTokens.Token(",")]+[params[j]]
            use = CTokens.QuickTokenize(f"(struct {self.name}*)eventer")+paramnamesuse
            ConstructorUse += f"{self.name}_Constructor({CTokens.Retriever(use).VomitAsText()});"
        params = CTokens.QuickTokenize("struct Dimension*dimension,iVector position")+params
        return params, ConstructorUse
    
    def getHeader(self):
        result = ""
        result += f"struct {self.name}" + "{struct Entity entity;" + f"{self.inside}" + "};"

        params, _ = self.getConstructorModifications()

        result += f"struct Entity* {self.name}_GetEntity_base({CTokens.Retriever(params).VomitAsText()});"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getHeader()
        return result
    
    def getSource(self):

        params, constrUse = self.getConstructorModifications()
        result = ""
        result += f"struct Entity* {self.name}_GetEntity_base({CTokens.Retriever(params).VomitAsText()})"
        result += "{"
        result += f"struct Entity* entity = GET_SIZED_STRUCT({self.name});"
        result += "entity->b.tbwgType = TBWG_TYPE_ENTITY;"
        result += f"entity->b.code = {self.codename};"
        result += f"entity->b.ID = getID();"
        result += f"entity->b.position = position;"
        result += f"entity->b.direction = getfVector(1.0f, 0.0f);"
        result += f"entity->b.dimension = dimension;"
        result += f"entity->b.collisionFunction = beingDefaultOneWayCollision;"
        result += f"entity->b.mass=1.0f;"

        result += "entity->b.visionHardness = 0;"
        values = "";i = 0;defaultEyes = [1.8, 1.0, 0.1, 1.0, 0.1]
        for k in ["eyeAngle","eyeLevel","eyeSpeed","hearingLevel","hearingSpeed"]:
            if k in self.changes: values += f"{CWriter.getAsCFloat(self.changes[k])},"
            else: values += f"{CWriter.getAsCFloat(defaultEyes[i])},"
            i+=1
        result += "struct Eye eye = { " + values + " };"
        result += "entity->b.baseEye = eye;"
        result += "entity->b.eye = eye;"

        result += f"for(unsigned int i = 0 ; i < EFFECT_TRIGGER_TYPE_COUNT ; i += 1)"
        result += "{entity->b.effects[i]=createList();}"

        result += "entity->b.baseQueue=createQueue();"

        # Queue

        result += "struct QueueEntityTurn* defaultEntityTurn = NEW(QueueEntityTurn);"
        result += "(*defaultEntityTurn)=getBasicEntityTurn(entity);"
        result += "queueAddTurn(&(entity->b.baseQueue), (struct QueueElementHeader*)defaultEntityTurn);"
        
        if "Eventer" in self.funcs: result += f"entity->eventer={self.funcs["Eventer"]};"
        else: result += "entity->eventer=defaultEntityEventer;"

        if "Constructor" in self.funcs: result += constrUse

        result += "return entity;"


        result += "}"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getSource()
        return result
    
    def getViewerPy(self,lang=""):
        explanation = self.getChange(f"explanation{lang}", self.getChange("explanation", "No explanation entered."))
        iconPath = self.getChange(f"icon{lang}", self.getChange(f"icon", defaultEntityIconPath))

        return getPythonViewFunctionCall("Entity", self.code, explanation, iconPath)
    
    def changeInvoke(self, name, value):
        self.changes[name] = value

class Effect:
    def __init__(self, name : str, inside):
        self.name = name
        self.inside = inside
        self.funcs = {}
        self.codename = f"EFFECT_CODENAME_{name.upper()}"
        self.code = 0

        self.changes = {}
    def getChange(self, name, default):
        if name in self.changes: return self.changes[name]
        else: return default
    def addFunction(self, name, func : Function):
        self.funcs[name] = func
        self.funcs[name].name.text = f"{self.name}_{name}"
        fn = self.funcs[name]
        if name == "Constructor":
            fetcherParam = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}")
            if len(self.funcs[name].parameters) > 0: fetcherParam+=[CTokens.Token(",")]
            self.funcs[name].parameters = fetcherParam+fn.parameters
        elif name in ["Executer"]:
            converter = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}=(struct {self.name}*)effectptr;")
            self.funcs[name].inside = converter+fn.inside
            self.funcs[name].parameters = CTokens.QuickTokenize(f"void* effectptr,")+self.funcs[name].parameters
        else:
            converter = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}=(struct {self.name}*)effectptr;")
            addition = CTokens.QuickTokenize(f"struct {self.name}*{fn.preparameters["naming"]}" + ("," if len(fn.parameters) != 0 else ""))
            self.funcs[name].parameters = addition+self.funcs[name].parameters
    
    def getConstructorModifications(self):
        ConstructorUse = ""
        params = []
        paramnamesuse = []
        if "Constructor" in self.funcs:
            params = self.funcs["Constructor"].parameters[4:]
            for j in range(1, len(params)):
                if j == len(params)-1 or params[j+1].type == CTokens.COMMA: paramnamesuse += [CTokens.Token(",")]+[params[j]]
            use = CTokens.QuickTokenize(f"(struct {self.name}*)effect")+paramnamesuse
            ConstructorUse += f"{self.name}_Constructor({CTokens.Retriever(use).VomitAsText()});"
        params = CTokens.QuickTokenize("void*effector")+params
        return params, ConstructorUse

    def getHeader(self):
        params, _ = self.getConstructorModifications()
        result = ""
        result += f"struct {self.name}" + "{struct Effect effect;" +  f"{self.inside}" + "};"
        result += f"struct Effect* {self.name}_GetEffect_base({CTokens.Retriever(params).VomitAsText()});"
        for s in self.funcs:
            fn = self.funcs[s]
            if type(fn) == StolenFunction: continue
            result += fn.getHeader()
        return result

    def getSource(self):
        params, ConstructorUse = self.getConstructorModifications()

        result = ""
        baseLinic = CTokens.QuickTokenize(f"struct Effect* {self.name}_GetEffect_base(")+params+[CTokens.Token(")")]
        result += CTokens.Retriever(baseLinic).VomitAsText()
        result += "{"
        result += f"struct Effect* effect = GET_SIZED_STRUCT({self.name});"
        result += f"effect->ID = getID();"
        result += f"TBWGType effectorType = TBWG_TYPE_UNKNOWN;"
        result += f"if(effector!=NULL)effectorType = *((TBWGType*)effector);"
        result += f"effect->effectorType=effectorType;"
        result += f"effect->effector=effector;"
        result += f"effect->willberemoved=0;"
        result += f"effect->code={self.codename};"


        if "time" in self.changes:
            result += f"effect->time={self.changes["time"]};"
        else:
            result += f"effect->time=-1;"
        result += f"effect->effectSpecs=0;"

        statNames = ["STR", "DEX", "CNS", "WIS", "SCS", "SPD","VISRES"]
        values = ""
        for k in ["STR", "DEX", "CNS", "WIS", "SCS", "SPD","VISRES"]:
            if k in self.changes: values += f"{self.changes[k]},"
            else: values += f"0,"
        values = values[:len(values)-1]

        result += "struct Stats s={" + values + "};"

        result += "effect->givenStats = s;"
        
        if "Executer" not in self.funcs:
            result += "effect->executer=defaultEffectExecuter;"
        else: 
            result += f"effect->executer={self.funcs["Executer"].name.text};"
        
        if "Constructor" in self.funcs:
            result += ConstructorUse

        result += "for (unsigned int i = 0 ; i < 8 ; i += 1) effect->details[i] = 0;"
        
        result += f"return effect;"

        result += "}"

        for func_name in self.funcs:
            fn = self.funcs[func_name]
            if type(fn) == StolenFunction: continue
            result += fn.getSource()

        return result
    def getViewerPy(self,lang=""):
        explanation = self.getChange(f"explanation{lang}", self.getChange("explanation", "No explanation entered."))
        iconPath = self.getChange(f"icon{lang}", self.getChange(f"icon", defaultEffectIconPath))
        
        return getPythonViewFunctionCall("Effect", self.code, explanation, iconPath)

    def changeInvoke(self, name, value):
        self.changes[name] = value


lastDefinition = None

# EFFECT DEFINITION

def EffectDefinition(retriever : CTokens.Retriever):
    global effects, lastDefinition

    structInside = retriever.getInsideOfNextBrackets(CTokens.CURLYBRACKET_OPEN)
    if structInside == None: return False
    name = retriever.get(2).text
    effects.append(Effect(name, CTokens.Retriever(structInside).VomitAsText()))
    _, end = retriever.getNextBracketIndices(CTokens.CURLYBRACKET_OPEN)
    retriever.jump(end+1)
    lastDefinition = effects[len(effects)-1]

# CHARACTER DEFINITION

def CharacterDefinition(retriever : CTokens.Retriever):
    global characters, lastDefinition

    structInside = retriever.getInsideOfNextBrackets(CTokens.CURLYBRACKET_OPEN)
    if structInside == None: return False
    name = retriever.get(2).text
    characters.append(Character(name, CTokens.Retriever(structInside).VomitAsText()))
    _, end = retriever.getNextBracketIndices(CTokens.CURLYBRACKET_OPEN)
    retriever.jump(end+1)
    lastDefinition = characters[len(characters)-1]

# EVENTER DEFINITION

def EventerDefinition(retriever : CTokens.Retriever):
    global eventers, lastDefinition

    structInside = retriever.getInsideOfNextBrackets(CTokens.CURLYBRACKET_OPEN)
    if structInside == None: return False
    name = retriever.get(2).text
    eventers.append(Eventer(name, CTokens.Retriever(structInside).VomitAsText()))
    _, end = retriever.getNextBracketIndices(CTokens.CURLYBRACKET_OPEN)
    retriever.jump(end+1)
    lastDefinition = eventers[len(eventers)-1]

def EntityDefinition(retriever : CTokens.Retriever):
    global entities, lastDefinition

    structInside = retriever.getInsideOfNextBrackets(CTokens.CURLYBRACKET_OPEN)
    if structInside == None: return False
    name = retriever.get(2).text
    entities.append(Entity(name, CTokens.Retriever(structInside).VomitAsText()))
    _, end = retriever.getNextBracketIndices(CTokens.CURLYBRACKET_OPEN)
    retriever.jump(end+1)
    lastDefinition = entities[len(entities)-1]

#IMPLEMENTATION

def Implementation(retriever : CTokens.Retriever):
    global lastDefinition
    
    targetName = retriever.get(2).text
    if targetName == "_": targetName = lastDefinition.name
    naming = retriever.get(3).text
    retriever.jump(3)
    functionName = None
    type = []
    while retriever.next():
        if retriever.get(1).checkme(CTokens.Token("(")):
            functionName = retriever.get(0)
            break
        else:
            type.append(retriever.get(0))
    parameterStart, parameterEnd = retriever.getNextBracketIndices(CTokens.BRACKET_OPEN)
    parameters = retriever.retrieveBetweenIndices(parameterStart,parameterEnd)
    retriever.jump(parameterEnd)
    definitionStart, definitionEnd = retriever.getNextBracketIndices(CTokens.CURLYBRACKET_OPEN)
    definition = retriever.retrieveBetweenIndices(definitionStart,definitionEnd)


    func = Function(type, functionName, parameters, definition, {"targetName":targetName, "naming":naming})

    for e in (effects+characters+eventers):
        if e.name == targetName:
            e.addFunction(functionName.text,func)


#STEALING

stealCommands = []
def Steal(retriever : CTokens.Retriever): # @stealingImplement GrabWalk Walk CanExecutedNow
    global lastDefinition, stealCommands
    targetName = retriever.get(2).text
    if targetName == "_": targetName = lastDefinition.name
    stealedfrom = retriever.get(3).text
    stealed = retriever.get(4).text
    print(f"{stealed} steal from {stealedfrom} to {targetName} entered to com list.")
    stealCommands.append( (targetName, stealedfrom, stealed) )
def ExecuteStealCommands():
    global stealCommands
    for com in stealCommands:
        targetName, stealedfrom, stealed = com
        prg = findDefByName(targetName)
        _stealedfrom = findDefByName(stealedfrom)
        if _stealedfrom == None: print(f"{stealedfrom} is empty.")
        prg.funcs[stealed] = StolenFunction(_stealedfrom.funcs[stealed].name)
        print(f"{stealed} steal from {stealedfrom} to {targetName}!")
    


def AddEventer(retriever : CTokens.Retriever):
    global lastDefinition

    
    targetName = retriever.get(2).text
    if targetName == "_": targetName = lastDefinition.name
    sourceName = retriever.get(3).text

    character = findDefByName(targetName)
    if type(character) != Character: print(f"Warning! trying to add eventer to a non character {type(character)} object in name of '{targetName}'")
    
    
    character.addEventerSource(sourceName)

def File(filepath):
    if filepath.split(".")[1] == "discard":
        print(filepath, " is discarded")
        return
    text = PP.do(filepath)
    c = CTokens.CharacterRetrieverFromText(text)
    #c  = Tokens.CharacterRetrieverFromFile("tbwgmanager.c")
    tokens = CTokens.AsTokens(c)

    retriever = CTokens.Retriever(tokens)

    def ChangeInvoke(name, value):
        global lastDefinition
        lastDefinition.changeInvoke(name, value)

    while retriever.next(usepasser = True):
        retrieved = retriever.retrieve(2)
        if not retrieved: break
        
        if retriever.get(0).checkme(CTokens.Token("@")):
            if retriever.get(1).checkme(CTokens.Token("effect",CTokens.DISTINCT)): 
                EffectDefinition(retriever)
            elif retriever.get(1).checkme(CTokens.Token("character",CTokens.DISTINCT)):
                CharacterDefinition(retriever)
            elif retriever.get(1).checkme(CTokens.Token("eventer",CTokens.DISTINCT)):
                EventerDefinition(retriever)
            elif retriever.get(1).checkme(CTokens.Token("entity", CTokens.DISTINCT)):
                EntityDefinition(retriever)
            elif retriever.get(1).checkme(CTokens.Token("implement",CTokens.DISTINCT)):
                Implementation(retriever)
            elif retriever.get(1).checkme(CTokens.Token("steal",CTokens.DISTINCT)):
                Steal(retriever)
            elif retriever.get(1).checkme(CTokens.Token("addeventer",CTokens.DISTINCT)):
                AddEventer(retriever)
            elif retriever.get(1).checkme(CTokens.Token("STOP", CTokens.DISTINCT)):
                break
            else: ChangeInvoke(retriever.get(1).text, retriever.get(2).text)

import os

def Folder(folderpath):

    lst = os.listdir(folderpath)
    for l in lst:
        nfolderpath = f"{folderpath}/{l}"
        if os.path.isfile(nfolderpath): File(nfolderpath)
        elif os.path.isdir(nfolderpath): Folder(nfolderpath)
        else: raise(f"Error on reaching to {nfolderpath}")


#retriever = CTokens.Retriever(funcdefs)

#print(retriever.VomitAsText())

mainfolder = "Project"
assetspath = f"{mainfolder}/Assets"
templates = f"{mainfolder}/templates"
codepath = f"{mainfolder}/Codes"

outsvpath = f"outsv"
outclpath = f"outcl"

Folder(codepath)
ExecuteStealCommands()

i = 0;codeDefinitor = ""
for e in effects:
    i += 1
    e.code = i
    codeDefinitor += f"#define {e.codename} {e.code} // effect {e.name}\n"
for e in characters:
    i += 1
    e.code = i
    codeDefinitor += f"#define {e.codename} {e.code} // character {e.name}\n"
for e in eventers:
    i += 1
    e.code = i
    codeDefinitor += f"#define {e.codename} {e.code} // eventer {e.name}\n"
for e in entities:
    i += 1
    e.code = i
    codeDefinitor += f"#define {e.codename} {e.code} // entity {e.name}\n"


print("inclusions:", PP.inclusions)

source = ""
header = ""

viewerspy_bylanguages = {}

for lang in ["","tr"]:
    viewerspy_bylanguages[lang] = ""

for ef in effects:
    source += ef.getSource()
    header += ef.getHeader()
    print("effect:", ef.name)

    for lang in viewerspy_bylanguages: viewerspy_bylanguages[lang] += "\t\t"+ef.getViewerPy(lang)
for c in eventers:
    source += c.getSource()
    header += c.getHeader()
    print("eventer:", c.name)
    for lang in viewerspy_bylanguages: viewerspy_bylanguages[lang] += "\t\t"+c.getViewerPy(lang)
for c in entities:
    source += c.getSource()
    header += c.getHeader()
    print("entity:", c.name)
    for lang in viewerspy_bylanguages: viewerspy_bylanguages[lang] += "\t\t"+c.getViewerPy(lang)
for c in characters:
    c.mergeEventersInOnePlace()
    source += c.getSource()
    header += c.getHeader()
    print("character:", c.name, f"({c.eventers})")
    for lang in viewerspy_bylanguages: viewerspy_bylanguages[lang] += "\t\t"+c.getViewerPy(lang)

# Blushes to Out

targetDirectory = "out"
morselsHeader = f"{outsvpath}/morsels"
src = f"{outsvpath}/src"

pushviewersdotpy = f"{outclpath}/pushviewers.py"

for p in [morselsHeader]:
    if not os.path.exists(p): os.makedirs(p)

with open(f"{morselsHeader}/morsels.h", "w") as file:
    file.write('#include "codes.h"\n')
    file.write(header+"\n\n"+source+"\n")
    file.write('#include "morsels_core.h"\n')

morsels_core_template = ""
with open(f"{templates}/morsels_core.h", "r") as file:
    morsels_core_template += file.read(-1)
morsels_core_template = CTokens.Retriever(ProcessInsideConversion(CTokens.QuickTokenize(morsels_core_template))).VomitAsText()

with open(f"{morselsHeader}/morsels_core.h", "w") as file:
    file.write(f"#define MORSELS_CHARACTER_COUNT {len(characters)}\n")

    grt = 'struct Character* morselsGetCharacterByOrder(unsigned int i, struct Dimension* dim, iVector position)\n'
    grt += '{\n'
    for i in range(len(characters)): 
        grt += f'if (i == {i}) return Get@{characters[i].name}(dim, position);\n'
    grt += '}\n'
    
    grtRetr = ProcessInsideConversion(CTokens.QuickTokenize(grt))
    file.write(CTokens.Retriever(grtRetr).VomitAsText())
    file.write(morsels_core_template)


with open(f"{morselsHeader}/codes.h","w") as file:
    file.write(codeDefinitor)


with open(pushviewersdotpy, "w", encoding="utf-8") as file:
    file.write(f"def pushGeneratedViews(viewManager, lang=''):\n")
    for lang in viewerspy_bylanguages:
        file.write(f"\tif lang == '{lang}':\n")
        file.write(viewerspy_bylanguages[lang])