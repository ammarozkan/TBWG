DISTINCT = 0
RANDOM = 1
SYMBOL = 2
TEXTTOKEN = 3
NUMBERS = 4
GREATTEXT = 5
BRACKET_OPEN = 6
BRACKET_CLOSE = 7
SQUAREBRACKET_OPEN = 8
SQUAREBRACKET_CLOSE = 9
CURLYBRACKET_OPEN = 10
CURLYBRACKET_CLOSE = 11
POINTER_ARROW = 22
ATSIGN = 23
COMMA = 32
LOWERCASE = 35
HIGHERCASE = 36
HASHTAG = 37
TWO_DOT = 38
THREE_DOT = 39
# NAMED
TYPENAME = 40
EMPTY = 120
NOTHING = 99
HEAVYENDLINE = 256

distinctKeys = "{}()<>=[]*+-.,;#/@"
numbers = "0123456789"
emptyKeys = " \n\r\t"
textToken = "'\""

multiCharSymbols = [POINTER_ARROW, TWO_DOT, THREE_DOT]
textCodes = [GREATTEXT, TYPENAME]

def isSymbol(code):
    if code in [BRACKET_CLOSE, BRACKET_OPEN, COMMA, LOWERCASE, HIGHERCASE, CURLYBRACKET_OPEN, CURLYBRACKET_CLOSE, HASHTAG, ATSIGN, SYMBOL] + multiCharSymbols: return True
    else: False

def isTypeName(name):
    if name in ["void","int","char","float"]: return True
    else: return False

def getCharType(c, prec=""):
    if prec+c == '->': return POINTER_ARROW
    elif prec+c == '..': return TWO_DOT
    elif prec+c == '...': return THREE_DOT
    elif c == '(': return BRACKET_OPEN
    elif c == ')': return BRACKET_CLOSE
    elif c == '[': return SQUAREBRACKET_OPEN
    elif c == ']': return SQUAREBRACKET_CLOSE
    elif c == '<': return LOWERCASE
    elif c == '>': return HIGHERCASE
    elif c == '{': return CURLYBRACKET_OPEN
    elif c == '}': return CURLYBRACKET_CLOSE
    elif c == ',': return COMMA
    elif c == '#': return HASHTAG
    elif c == '@': return ATSIGN
    elif c in textToken: return TEXTTOKEN
    elif c in numbers: return NUMBERS
    elif c in distinctKeys: return SYMBOL
    elif c in emptyKeys: return EMPTY
    else: return GREATTEXT

def getTextType(text):
    if isTypeName(text): return TYPENAME
    else: return getCharType(text)

class Token:
    def __init__(self, inp, _type = None):
        if type(inp) == str: 
            self.text = inp
            self.type = getTextType(inp) if _type == None else _type
            return
        elif type(inp) == int:
            self.text = None
            self.type = inp
        else:
            raise("What is that token input type?")
    def checkme(self, filter):
        if filter.type == DISTINCT:
            return filter.text == self.text
        elif filter.type == RANDOM:
            return True
        elif filter.type == TYPENAME:
            return self.type == TYPENAME
        elif filter.type == GREATTEXT:
            return self.type != NUMBERS and not isSymbol(self.type)
        else: return filter.type == self.type
    def __str__(self):
        return f"[Token {self.type} | {self.text}]"

def stringisizeTokens(tokens):
    if not tokens: return ""
    result = ""
    for token in tokens:
        result += str(token)
    return result

class Retriever:
    def __init__(self, tokens):
        self.tokens = tokens
        self._from = 0
        self.passer = 1
        pass

    def makeNothingsDisappear(self):
        i = 0
        while i < len(self.tokens):
            if self.tokens[i].type == NOTHING:
                self.tokens.remove(self.tokens[i])
                i -= 1
            i += 1
    
    def getNears(self):
        start = max(0, self._from-6)
        end = min(len(self.tokens)-1, self._from+6)

        res = ""
        for token in self.tokens[start:end]:
            res += token.text
        return res
        

    def correct(self, _index):
        return self._from + _index <= len(self.tokens)
    def get(self, _index):
        if not self.correct(_index): return None
        return self.tokens[self._from + _index]
    def modify(self, _index, newvalue):
        if not self.correct(_index): return False

        if type(newvalue) == Token:
            self.tokens[self._from+_index] = newvalue
        elif type(newvalue) == list:
            self.tokens = self.tokens[0:self._from+_index] + newvalue + self.tokens[self._from+_index+1:]
        return True
    def insert(self, _index, newvalue): # AFTER the index
        if not self.correct(_index): return False

        if type(newvalue) != list:
            newvalue = [newvalue]
        
        self.tokens = self.tokens[0:self._from+_index+1] + newvalue + self.tokens[self._from+_index+1:]
        return True
    def retrieve(self, _count):
        if _count + self._from < len(self.tokens):
            return [self.tokens[self._from + i] for i in range(0,_count)]
    def retrieveBetweenIndices(self, _start, _end):
        if not self.correct(_end) or not self.correct(_start+1): return None
        else: return self.tokens[self._from+_start+1:self._from+_end]
    def next(self, usepasser = False):
        if usepasser: 
            if self.passer == 1:
                self.passer -= 1
                return True
        
        self._from += 1
        return self._from < len(self.tokens)
    def jump(self,_indice):
        self._from+=_indice
    def check(self, tokenlist):
        ln = len(tokenlist)
        toks = self.retrieve(ln)
        if toks == None: return False

        for i in range(0,ln):
            if not toks[i].checkme(tokenlist[i]): return False
        
        return True
    def getNextBracketIndices(self, bracketCode): # bracketOpen = bracketCode, bracketClose = bracketCode + 1
        indent = 0
        indentUpgraded = False

        start = None
        end = None
        for i in range(0, len(self.tokens) - self._from):
            token = self.get(i)
            if token.type == bracketCode:
                indent += 1
                if indentUpgraded == False:
                    start = i
                indentUpgraded = True
            elif token.type == bracketCode + 1: indent -= 1

            if indent == 0 and indentUpgraded: 
                end = i
                break
        
        return (start, end) if start != None and end != None else None
    
    def getInsideOfNextBrackets(self, bracketCode):
        start, end = (None, None)
        res = self.getNextBracketIndices(bracketCode)
        if res == None or None in res: return None
        else: start, end = res

        return self.retrieveBetweenIndices(start,end)
    
    def getThroughBackUntilEnds(self):
        i = 0
        result = []
        while self.correct(-i):
            token = self.get(i)
            if token.checkme(Token(";")) or token.checkme(Token("}")): break
            else: result += token
            i += 1
        return result

    def VomitAsText(self,aftercommaseperator=True):
        text = ""
        lastCode = NOTHING
        for token in self.tokens:
            if token.type == HEAVYENDLINE: text += "\n"
            elif token.type in textCodes and lastCode in textCodes: text += " "+token.text
            elif token.type == NUMBERS and lastCode in textCodes: text += " "+token.text
            elif token.type == TEXTTOKEN: text += f'"{token.text}"'
            elif token.text == ';' and aftercommaseperator: text += f'{token.text}\n'
            else: text += token.text
            lastCode = token.type
        return text

    





class CharacterRetriever:
    def __init__(self):
        pass
    def get():
        return None
    
class CharacterRetrieverFromText(CharacterRetriever):
    def __init__(self, text):
        self.text = text
        self.indice = 0
    def get(self):
        if self.indice == len(self.text): return None
        else:
            result = self.text[self.indice]
            self.indice += 1
            return result

class CharacterRetrieverFromFile(CharacterRetriever):
    def __init__(self, filepath):
        self.file = open(filepath, 'r')
    def get(self):
        return self.file.read(1)

def AsTokens(retriever : CharacterRetriever, optionalTextOpeners = []):
    result = []

    memory = ""
    lasttype = None
    textin = False
    textin_key = None
    textin_optionalopener = None

    def AfterBigIndex(c):
        nonlocal textin, textin_key, textin_optionalopener, lasttype, memory
        if c in emptyKeys:
            if lasttype != None:
                if lasttype == GREATTEXT:
                    if isTypeName(memory):
                        lasttype = TYPENAME
                result.append(Token(memory, lasttype))
                memory = ""
                lasttype = None
            return True

        
        elif getCharType(c) == TEXTTOKEN or getCharType(c) in optionalTextOpeners:
            textin = True
            if getCharType(c) == TEXTTOKEN: textin_key = c
            else: textin_key = None

            if getCharType(c) in optionalTextOpeners: textin_optionalopener = getCharType(c)
            else: textin_optionalopener = None

            #result.append(Token(memory, lasttype))
            memory = ""
            lasttype = TEXTTOKEN
        
        elif lasttype == None:
            lasttype = getCharType(c)
            memory = "" + c
        elif lasttype in textCodes and getCharType(c) == NUMBERS:
            memory += c
        elif lasttype != getCharType(c):
            if lasttype == GREATTEXT:
                if isTypeName(memory):
                    lasttype = TYPENAME
            result.append(Token(memory, lasttype))
            memory = ""+c
            lasttype = getCharType(c)
        else:
            lasttype = getCharType(c)
            memory += c
        
        return False
    

    while (c := retriever.get()):
        
        if textin:
            if getCharType(c) == TEXTTOKEN and textin_optionalopener == None and c == textin_key:
                textin = False
                result.append(Token(memory, TEXTTOKEN))
                memory = ""
                lasttype = None
            elif getCharType(c) == TEXTTOKEN and textin_optionalopener != None and getCharType(c) == textin_optionalopener + 1:
                textin = False
                result.append(Token(memory, TEXTTOKEN))
                memory = ""
                lasttype = None
            else:
                if c not in '\n':
                    memory += c
        elif getCharType(c, memory) in multiCharSymbols:
            lasttype = getCharType(c, memory)
            memory += c
        elif isSymbol(lasttype):
            result.append(Token(memory, lasttype))
            lasttype = getCharType(c)
            memory = ""
            lasttype = None
            if AfterBigIndex(c): continue
        else:
            if AfterBigIndex(c): continue
        
        
        
    if lasttype != None: result.append(Token(memory, lasttype))
    return result


def QuickTokenize(text):
    return AsTokens(CharacterRetrieverFromText(text))