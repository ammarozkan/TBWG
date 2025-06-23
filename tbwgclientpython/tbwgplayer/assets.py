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
    def addEventer(self, name, path):
        self.images["eventer"+str(name)] = path
    def addEffect(self, code, path):
        self.images["effect"+str(code)] = path
    def addBackground(self, name, path):
        self.images["background"+str(name)] = path
    def addSound(self, name, path):
        self.sounds[f"{name}"] = path
    
    def getCharacter(self, ID):
        return self.get("character"+str(ID))
    def getEventer(self, name):
        return self.get("eventer"+str(name))
    def getEffect(self, code):
        return self.get("effect"+str(code))
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
