import pygame


def init():
    pygame.init()
    pygame.font.init() # you have to call this at the start, 
                    # if you want to use this module.
    

REQ_NOTHING = 0
REQ_POSITION = 1
REQ_DIRECTION = 2
REQ_AREA = 3
REQ_EVENTER = 4
REQ_EVENTER_MID = 5
REQ_BUSY = 1024

testimage = pygame.image.load("please.gif")
begimage = pygame.image.load("beg.jpg")
defaultbuttonimage = begimage


class Camera:
    def __init__(self):
        self.x = 0
        self.y = 0

        self.scalex = 1
        self.scaley = 1

        self.grp = self.getRelativePos
        self.grl = self.getRelativeLength
        self.ugrp = self.getUnRelativePos
        self.ugrl = self.getUnRelativeLength
    
    def getRelativePos(self, x, y):
        return self.scalex*(x-self.x), self.scaley*(y - self.y)
    def getRelativeLength(self, x, y):
        return self.scalex*x, self.scaley*y
    def getUnRelativePos(self, x, y):
        return (x/self.scalex + self.x), (y/self.scaley + self.y)
    def getUnRelativeLength(self, x, y):
        return x/self.scalex, y/self.scaley
    def closer(self):
        self.x*=self.scalex
        self.y*=self.scaley
        self.scalex += 0.1
        self.scaley += 0.1
        self.x/=self.scalex
        self.y/=self.scaley
    def further(self):
        self.x*=self.scalex
        self.y*=self.scaley
        self.scalex -= 0.1
        self.scaley -= 0.1
        self.x/=self.scalex
        self.y/=self.scaley
    
    def move(self, x, y):
        self.x += x
        self.y += y

class Assets:
    def __init__(self):
        self.images = {}
    def addCharacter(self, ID, path):
        self.images["character"+str(ID)] = path
    
    def getCharacter(self, ID):
        return self.images["character"+str(ID)]
    
    def addEventer(self, name, path):
        self.images["eventer"+str(name)] = path
    
    def getEventer(self, name):
        return self.images["eventer"+str(name)]
    
    def add(self, name, path):
        self.images[str(name)] = path
    def get(self, name):
        return self.images[str(name)]
    
    def loadAll(self):
        newimages = {}
        for n in self.images:
            newimages[n] = pygame.image.load(self.images[n])
        self.images = newimages

def buttonclickdefault(id):
    print(f"{id} clicked!")

class Button:
    def __init__(self, code, pos, size, scale, image, onclick = buttonclickdefault):
        self.code = code
        self.pos = pos
        self.size = size
        self.scale = scale
        self.image = image

        self.updaterect()

        self.onclick = onclick
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
            if self.pos[i]-self.size[i]/2 > x: return False
            if self.pos[i]+self.size[i]/2 < x: return False
        return True
    def eventcontrol(self, event):
        if event == None: return event
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 3 and self.controlclickpos(event): self.onclick(self.code)
            return None
        elif event.type == pygame.MOUSEBUTTONUP:
            pass
        return event

class UITool:
    def __init__(self, width, height, buttonsize=70):
        self.points = {}
        self.width = width
        self.height = height
        self.buttongap = 10
        self.buttonsize = buttonsize

        self.points["bottom"] = (self.width/2, self.height)
        self.points["bottom-buttonsize"] = (self.width/2, self.height-self.buttonsize)
    
    def getButtons(self, count, pointname, onclick):
        result = []
        part = self.buttonsize + self.buttongap
        pos = self.points[pointname]
        firstbuttonpos = (pos[0]-part*count/2,pos[1])
        for i in range(0, count):
            p = (firstbuttonpos[0]+part*i,firstbuttonpos[1])
            result.append(Button(i, p, (self.buttonsize,self.buttonsize), 1, defaultbuttonimage, onclick))
        return result
    
    def setPoint(self, name, x, y):
        self.points[name] = (x,y)

class TBWGPyGamePlayer:
    def __init__(self, ScreenWidth, ScreenHeight, gridsize = 50, assets = Assets()):
        self.currentrequest = REQ_NOTHING
        self.lastrequest_answer = None
        self.requestinfo = None

        self.screenWidth = ScreenWidth
        self.screenHeight = ScreenHeight
        
        self.screen = pygame.display.set_mode((ScreenWidth,ScreenHeight))
        self.run = True
        self.camera = Camera()

        self.gridsize = gridsize

        self.initcameracontrols()
        
        self.font = pygame.font.SysFont('Comic Sans MS', 30)

        self.characters = []

        self.assets = assets

        self.uitool = UITool(ScreenWidth, ScreenHeight)
        self.buttons = []
    
    def request(self, reqtype, info = None):
        if self.currentrequest != REQ_NOTHING: return REQ_BUSY
        self.currentrequest = reqtype
        self.requestinfo = info
        while self.currentrequest != REQ_NOTHING: pass
        return self.lastrequest_answer
    
    def answerrequest(self, answer):
        self.lastrequest_answer = answer
        self.currentrequest = REQ_NOTHING
        return

    def getGridRect(self, x, y):
        x,y = x*self.gridsize, y*self.gridsize
        
        px, py, sx, sy = x+1,y+1,self.gridsize-2,self.gridsize-2
        px, py = self.camera.grp(px,py)
        sx, sy = self.camera.grl(sx,sy)
        return (px,py,sx,sy)
    
    def drawTestGrid(self, x, y):
        rect = pygame.Rect(*(self.getGridRect(x,y)))
        pygame.draw.rect(self.screen, pygame.Color(100,100,100), rect)
        self.drawText(str(f"{x},{y}"),(rect.x, rect.y))
    
    def drawImageGrid(self, image, x, y):
        px, py, sx, sy = self.getGridRect(x,y)
        image = pygame.transform.scale(image, (sx, sy))
        rect = pygame.Rect(px,py,sx,sy)
        self.screen.blit(image,rect)

    def initcameracontrols(self):
        self.cc = False
        self.last_cc_pos = (0,0)
    
    def cameracontrol(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            self.last_cc_pos = event.pos
            self.cc = True
        elif event.type == pygame.MOUSEBUTTONUP:
            self.cameramover(event.pos)
            self.cc = False
    
    def cameramover(self, mousepos):
        dx = mousepos[0]-self.last_cc_pos[0]
        dy = mousepos[1]-self.last_cc_pos[1]
        dx, dy = self.camera.ugrl(dx,dy)
        self.last_cc_pos = mousepos
        self.camera.move(-dx,-dy)
    
    def cameracontrolloop(self, mousepos):
        if self.cc:
            self.cameramover(mousepos)
    
    
    def getclickedGrid(self, x, y):
        x,y = self.camera.ugrp(x,y)
        gx, gy = int(x/self.gridsize),int(y/self.gridsize)
        if x < 0: gx -= 1
        if y < 0: gy -= 1
        return gx, gy
    
    def click(self, event):
        event = self.UIClickHandler(event)
        if event == None: return
        cg = self.getclickedGrid(*event.pos)
        if event.type == pygame.MOUSEBUTTONDOWN: self.GridClick(*cg)

        
    
    def event(self, event):
        if event.type in [pygame.MOUSEBUTTONDOWN,pygame.MOUSEBUTTONUP]:
            if event.button == 2: self.cameracontrol(event)
            if event.button == 4: self.camera.closer()
            if event.button == 5: self.camera.further()
            if event.button == 3: self.click(event)
        if event.type == pygame.KEYDOWN:
            if event.unicode == 'r': self.camera.x, self.camera.y = 0, 0
        if event.type == pygame.QUIT:
            self.run = False
    
    def events(self):
        for event in pygame.event.get():
            self.event(event)
    

    def drawUI(self):
        for button in self.buttons:
            button.draw(self.screen)
        pass
    
    def eventeronclick(self, id):
        if self.currentrequest != REQ_EVENTER_MID: return
        if id == -1: self.answerrequest(None)
        self.answerrequest(id)
        buttons = []
    
    def UIClickHandler(self, event):
        event = event
        for button in self.buttons:
            event = button.eventcontrol(event)
        return event
    
    def GridClick(self, x, y):
        print(x,y)
        if self.currentrequest == REQ_POSITION:
            self.answerrequest((x,y))
        pass
    
    def drawText(self, text, pos):
        text_surface = self.font.render(text, False, (255,50,50))
        self.screen.blit(text_surface, pos)
    
    def drawReq(self):
        text = "No Requests"

        if self.currentrequest == REQ_POSITION: text = "Position Request!"
        elif self.currentrequest == REQ_DIRECTION: text = "Direction Request!"
        elif self.currentrequest == REQ_AREA: text = "Area Request!"
        elif self.currentrequest == REQ_EVENTER_MID: text = "Eventer Mid!"
        elif self.currentrequest == REQ_EVENTER: text = "Eventer request!"


        self.drawText(text, (0,0))
    
    def drawcharacters(self,characters):
        for c in characters:
            x,y = c.position.x, c.position.y
            img = self.assets.getCharacter(c.characterCode)
            self.drawImageGrid(img, x, y)
            self.drawText(str(c.hp), (x,y))


    def draw(self):
        for x in range(-3,3):
            for y in range(-3,3):
                self.drawTestGrid(x,y)
        
        self.drawcharacters(self.characters)
        self.drawReq()
        self.drawUI()
        
    def loop(self):
        while self.run:
            mousepos = pygame.mouse.get_pos()
            self.events()
            self.cameracontrolloop(mousepos)

            if self.currentrequest == REQ_EVENTER:
                i = 0
                nbuttons = self.uitool.getButtons(len(self.requestinfo), "bottom-buttonsize", self.eventeronclick)
                self.buttons = nbuttons
                self.currentrequest = REQ_EVENTER_MID

            self.screen.fill((0, 0, 0))
            self.draw()
            pygame.display.flip()