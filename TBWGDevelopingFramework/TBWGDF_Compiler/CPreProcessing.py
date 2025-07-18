import CTokens

class PreProcessor:
    def __init__(self):
        self.inclusions = []

    def CommentOut(self, line, onmultilinecomment=False):
        result = ""

        twoc = ""
        for c in line:
            if not onmultilinecomment: result += c
            
            twoc += c
            if len(twoc) > 2:
                twoc = twoc[1:]
            elif len(twoc) < 2:
                continue


            if twoc == "//": 
                result = result[:len(result)-2]
                break
            elif twoc == "*/" and onmultilinecomment:
                onmultilinecomment = False
            elif twoc == "/*" and not onmultilinecomment:
                onmultilinecomment = True
            elif twoc[0] == '\\' and twoc != '\\\\':
                exceptional = result[len(result)-1]
                result = result[:len(result)-2]
                result += exceptional

        
        return result, onmultilinecomment
    
    def ProcessPreProcessCommand(self, tokens):
        if tokens[1].checkme(CTokens.Token("include")):
            path = CTokens.Retriever(tokens).getInsideOfNextBrackets(CTokens.LOWERCASE)
            if path == None: raise("Empty or not ended include path.")
            self.inclusions.append(CTokens.Retriever(path).VomitAsText())
        return None

    def ProcessLine(self, line):
        tokens = CTokens.AsTokens(CTokens.CharacterRetrieverFromText(line), [])
        if len(tokens) == 0: return
        if tokens[0].checkme(CTokens.Token(CTokens.HASHTAG)):
            return self.ProcessPreProcessCommand(tokens)
        return line


    def do(self, filepath):
        text = ""
        with open(filepath, "r", encoding="utf-8") as file:
            onmultilinecomment = False
            print(file)
            for line in file:
                line, onmultilinecomment = self.CommentOut(line, onmultilinecomment)
                proc = self.ProcessLine(line)
                if type(proc) == str: text += proc
        return text