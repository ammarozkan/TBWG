import pygame

def buttonclickdefault(id):
    print(f"{id} clicked!")

class Button:
    def __init__(self, code, pos, size, scale, image, onclick = buttonclickdefault):
        self.code = code
        self.pos = pos
        if type(size) != tuple: size = (size,size)
        self.size = size
        self.scale = scale
        self.image = image

        self.updaterect()

        self.onclick = onclick
    def updateimage(self, image):
        self.image = image
        self.updaterect()
    def updaterect(self):
        px, py = (self.pos[0]-self.size[0]/2, self.pos[1]-self.size[1]/2)
        sx, sy = self.size
        self.image = pygame.transform.scale(self.image, (sx,sy))
        self.rect = pygame.Rect(px,py,sx,sy)
    def draw(self, screen):
        screen.blit(self.image,self.rect)
    def setuniques(self, code, image):
        self.code = code
        self.image = image
    def controlclickpos(self, event):
        x,y = event.pos
        
        for i in range(0,2):
            if self.pos[i]-self.size[i]/2 > event.pos[i]: return False
            if self.pos[i]+self.size[i]/2 < event.pos[i]: return False
        return True
    def eventcontrol(self, event):
        if event == None: return event
        if event.type == pygame.MOUSEBUTTONUP:
            if self.controlclickpos(event): 
                self.onclick(self.code)
                return None
        return event

class IconedText:
    def __init__(self, image, text, pos, size):
        self.text = text
        self.image = image
        self.pos = pos
        if type(size) != tuple: size = (size, size)
        self.size = size
        self.updaterect()
    def updaterect(self):
        px, py = (self.pos[0]-self.size[0]/2, self.pos[1]-self.size[1]/2)
        sx, sy = self.size
        self.image = pygame.transform.scale(self.image, (sx,sy))
        self.rect = pygame.Rect(px,py,sx,sy)
    def updateimage(self, image):
        self.image = image
        self.updaterect()
    def updatesize(self, size):
        self.size = size
        self.updaterect()
    def updatepos(self, pos):
        self.pos = pos
        self.updaterect()
    def draw(self, screen, font, textcolor = (255,50,50)):
        screen.blit(self.image, self.rect)
        text_surface = font.render(self.text, False, textcolor)
        tp = (self.pos[0]-text_surface.get_size()[0]/2,self.pos[1]-text_surface.get_size()[1]/2)
        screen.blit(text_surface, tp)

class FillingBar:
    def __init__(self):
        pass
    def setValue(self, v, max):
        self.value = v
        self.max = max
    def draw(self, screen, pos):
        pass

class UITool:
    def __init__(self, width, height, defaultbuttonimage, buttonsize=70):
        self.points = {}
        self.width = width
        self.height = height
        self.buttongap = 10
        self.buttonsize = buttonsize

        self.points["bottom"] = (self.width/2, self.height)
        self.points["bottom-buttonsize"] = (self.width/2, self.height-self.buttonsize)
        self.points["upright-buttonsize"] = (self.width - self.buttonsize, 0 + self.buttonsize)
        self.points["bottomleft"] = (0, self.height)
        self.points["middle"] = (self.width/2, self.height/2)
        self.defaultbuttonimage = defaultbuttonimage
    
    def getButtons(self, count, pointname, onclick, transform = (0,0)):
        result = []
        part = self.buttonsize + self.buttongap
        pos = self.points[pointname]
        pos = (pos[0]+transform[0],pos[1]+transform[1])
        firstbuttonpos = (pos[0]-(count-1)*part/2,pos[1])
        for i in range(0, count):
            p = (firstbuttonpos[0]+part*i,firstbuttonpos[1])
            result.append(Button(i, p, (self.buttonsize,self.buttonsize), 1, self.defaultbuttonimage, onclick))
        return result
    
    def getButton(self, image, code, pointname, onclick, transform = (0,0)):
        pos = self.points[pointname]
        pos = (pos[0]+transform[0],pos[1]+transform[1])
        return Button(code, pos, self.buttonsize, 1, image, onclick)
    
    def setPoint(self, name, x, y):
        self.points[name] = (x,y)
    
    def getIconedText(self, count, size, gap, defaultimage, pointname, transform=(0,0)):
        result = []
        part = size + gap
        pos = self.points[pointname]
        pos = (pos[0]+transform[0],pos[1]+transform[1])
        firstpos = (pos[0]-(count-1)*part/2,pos[1])
        for i in range(0, count):
            p = (firstpos[0]+part*i,firstpos[1])
            result.append(IconedText(defaultimage, "a", p, size))
        return result