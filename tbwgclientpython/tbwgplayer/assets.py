import pygame

class Assets:
    def __init__(self):
        self.images = {}
        self.sounds = {}
    def addError(self,path):
        self.images["error"] = path
    def add(self, name, path):
        self.images[str(name)] = path
    def addCharacter(self, ID, path):
        self.images["character"+str(ID)] = path
    def addEventer(self, code, path):
        self.images["eventer"+str(code)] = path
    def addEntity(self,code,path):
        self.images["entity"+str(code)] = path
    def addEffect(self, code, path):
        self.images["effect"+str(code)] = path
    def addWorldEvent(self, name, path):
        self.images["worldevent"+str(name)] = path
    def addBackground(self, name, path):
        self.images["background"+str(name)] = path
    def addSound(self, name, path):
        self.sounds[f"{name}"] = path
    
    def getCharacter(self, ID):
        return self.get("character"+str(ID))
    def getEventer(self, code):
        return self.get("eventer"+str(code))
    def getEffect(self, code):
        return self.get("effect"+str(code))
    def getEntity(self,code):
        return self.get("entity"+str(code))
    def getWorldEvent(self, name):
        return self.get("worldevent"+str(name))
    def getBackground(self, name):
        return self.get("background"+str(name))
    def get(self, name):
        if str(name) not in self.images:
            return self.images["error"]
        else:
            return self.images[str(name)]

    def getSound(self, name):
        if str(name) not in self.sounds:
            return self.sounds["error"]
        else:
            return self.sounds[name]
    
    def loadAll(self):
        newimages = {}
        for n in self.images:
            newimages[n] = pygame.image.load(self.images[n])
        self.images = newimages

        newsounds = {}
        for n in self.sounds:
            newsounds[n] = pygame.mixer.Sound(self.sounds[n])
        self.sounds = newsounds


class ViewerManager:
    def __init__(self, assets):
        self.eventers = {}
        self.effects = {}
        self.entities = {}
        self.characters = {}
        self.buttons = {}
        self.worldevents = {}
        self.assets = assets
    def PushEventerView(self, code, explanation, imgpath):
        self.assets.addEventer(code, imgpath)
        self.eventers[str(code)] = explanation
    def PushEffectView(self, code, explanation, imgpath):
        self.assets.addEffect(code, imgpath)
        self.effects[str(code)] = explanation
    def PushEntityView(self, code, explanation, imgpath):
        self.assets.addEntity(code, imgpath)
        self.entities[str(code)] = explanation
    def PushCharacterView(self, code, explanation, imgpath):
        self.assets.addCharacter(code, imgpath)
        self.characters[str(code)] = explanation
    def PushButtonView(self, code, explanation, imgpath):
        self.assets.add(code, imgpath)
        self.buttons[str(code)] = explanation
    def PushWorldEventView(self, code, explanation, imgpath, soundpath):
        self.assets.addWorldEvent(code, imgpath)
        self.assets.addSound(code, soundpath)
        self.worldevents[code] = explanation
    
    def GetEventerView(self, code):
        return self.assets.getEventer(code), self.eventers[str(code)]
    def GetCharacterView(self, code):
        return self.assets.getCharacter(code), self.characters[str(code)]
    def GetEffectView(self, code):
        return self.assets.getEffect(code), self.effects[str(code)]
    def GetEventerImg(self, code):
        return self.assets.getEventer(code)

    def GetButtonView(self, code):
        return self.assets.get(code), f"{code}"