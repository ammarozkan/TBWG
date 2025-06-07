import pygame


def init():
    pygame.init()
    pygame.font.init()  # you have to call this at the start, 
                        # if you want to use this module.


REQ_NOTHING = 0
REQ_POSITION = 1
REQ_DIRECTION = 2
REQ_AREA = 3
REQ_EVENTER = 4
REQ_EVENTER_MID = 5
REQ_CHARACTER_SELECTION = 6
REQ_CHARACTER_SELECTION_MID = 7
REQ_BUSY = 1024

from .camera import *
from .assets import *
from .ui import *

import math
def vlength(v):
    return math.sqrt(v[0]**2 + v[1]**2)
def normalize(vector):
    v = vector
    length = vlength(v)
    if (length == 0): return (0,0)
    return (v[0]/length, v[1]/length)

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
        
        self.font = pygame.font.SysFont('Comic Sans MS', 24)

        self.characters = []

        self.observer = None

        self.oneffects = []

        self.assets = assets

        self.uitool = UITool(ScreenWidth, ScreenHeight,assets.get("DefaultButton"))

        self.eventerbuttons = []
        self.characterchoicebuttons = []
        self.buttons = []
        self.buttons.append( self.uitool.getButton(assets.get("ClearWorldEvents"), 0, "bottomleft", self.clearWorldEvents, transform=(32,-32)) )

        self.worldevents = []
        
        self.restuses = self.uitool.getIconedText(5, 32, 20, self.assets.get("DefaultRestUses"), "bottom", transform=(0,-32))

        self.energy = (0,0)
        self.spellenergy = (0,0)

        self.gridhpshower = IconedText(assets.get("HealthIcon"), "0", (0,0), (20,20))
        self.effectshower = IconedText(assets.get("DefaultEffectIcon"), "0", (0,0), (50,50))

        self.directionchoosestate = None

        self.background = self.assets.getBackground("default")
    
    def updateBackground(self):
        self.background = pygame.transform.scale(self.background, (self.screenWidth, self.screenHeight))
        rect = pygame.Rect(0, 0, self.screenWidth,self.screenHeight)
    
    def changeBackground(self, image):
        self.background = image
        self.updateBackground()
    
    def clearWorldEvents(self, code):
        print("World events cleared.")
        self.worldevents = []
    
    def setRestUses(self, uses):
        for i in range(0,len(uses)):
            self.restuses[i].text = str(uses[i])
    
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
    
    def getGridPoint_(self, x, y, pointname):
        r = self.getGridRect(x,y)

        if (pointname == "bottom"): return (r[0] + r[3]/2, r[1]+r[3])
        elif (pointname == "bottomleft"): return (r[0], r[1]+r[3])
        elif (pointname == "middle"): return r[0] + r[3]/2, r[1] + r[3]/2
        
        return r[0] + r[3]/2, r[1] + r[3]/2
    
    def getGridPoint(self, x, y, pointname, transform = (0,0)):
        px, py = self.getGridPoint_(x, y, pointname)
        return (px+transform[0],py+transform[1])

    # CAMERA CONTROL

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

    
    # EVENT

    def directionrequestEvents(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            self.directionchoosestate = event.pos
        elif event.type == pygame.MOUSEBUTTONUP:
            v1 = self.directionchoosestate
            v2 = event.pos
            direction = (v2[0]-v1[0], v2[1]-v1[1])
            self.directionchoosestate = None
            if vlength(direction) == 0: return
            direction = normalize(direction)
            self.answerrequest(direction)
    
    def event(self, event):
        if event.type in [pygame.MOUSEBUTTONDOWN,pygame.MOUSEBUTTONUP]:
            if self.currentrequest == REQ_DIRECTION and event.button == 1: self.directionrequestEvents(event)
            if event.button == 2: self.cameracontrol(event)
            elif event.button == 4: self.camera.closer()
            elif event.button == 5: self.camera.further()
            elif event.button == 1: self.click(event)
        if event.type == pygame.KEYDOWN:
            if event.unicode == 'r': self.camera.x, self.camera.y = 0, 0
        if event.type == pygame.QUIT:
            self.run = False
    
    def events(self):
        for event in pygame.event.get():
            self.event(event)
    
    def eventeronclick(self, id):
        if self.currentrequest != REQ_EVENTER_MID: return
        if id == -1: self.answerrequest(None)
        self.answerrequest(id)
        self.eventerbuttons = []
    
    def characterchoiceonclick(self, id):
        self.answerrequest(id)
        self.characterchoicebuttons = []
    
    def UIClickHandler(self, event):
        event = event
        if event == None: return event
        for button in self.eventerbuttons:
            event = button.eventcontrol(event)
        for button in self.characterchoicebuttons:
            event = button.eventcontrol(event)
        for button in self.buttons:
            event = button.eventcontrol(event)
        return event
    
    def GridClick(self, x, y):
        if self.currentrequest == REQ_POSITION:
            print("Answering request ",(x,y))
            self.answerrequest((x,y))
    

    # DRAW

    def drawBackground(self):
        self.screen.blit(self.background,pygame.Rect(0,0,self.screenWidth,self.screenHeight))
    
    def drawTestGrid(self, x, y):
        self.drawColorGrid(x,y,(100,100,100))
        self.drawText(str(f"{x},{y}"),(rect.x, rect.y))
    
    def drawColorGrid(self, x, y, color):
        rect = pygame.Rect(*(self.getGridRect(x,y)))
        pygame.draw.rect(self.screen, pygame.Color(*color), rect)
    
    def drawImageGrid(self, image, x, y):
        px, py, sx, sy = self.getGridRect(x,y)
        image = pygame.transform.scale(image, (sx, sy))
        rect = pygame.Rect(px,py,sx,sy)
        self.screen.blit(image,rect)
    
    def drawTextToGrid(self, text, grid, pos):
        text_surface = self.font.render(text,False,(255,50,50))
        px, py, sx, sy = self.getGridRect(*grid)
        self.screen.blit(text_surface,(px,py))
    
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
    
    def drawBars(self):
        p1 = self.uitool.points["bottomleft"]
        p1 = (p1[0],p1[1]-100)
        self.drawText(str(self.energy), p1)
        p2 = (p1[0],p1[1]-40)
        self.drawText(str(self.spellenergy), p2)
    
    def drawEffects(self):
        i = 0
        for ef in self.oneffects:
            self.drawText(ef, (0,i*50))
            self.effectshower.updatepos((30,i*50+30))
            self.effectshower.updateimage(self.assets.getEffect(ef))
            self.effectshower.draw(self.screen, self.font)
            i += 1
    
    def drawUI(self):
        for button in self.eventerbuttons:
            button.draw(self.screen)
        for button in self.characterchoicebuttons:
            button.draw(self.screen)
        for button in self.buttons:
            button.draw(self.screen)
        if self.currentrequest == REQ_EVENTER_MID: 
            for r in self.restuses: r.draw(self.screen, self.font)
        self.drawReq()

        self.drawBars()
        self.drawEffects()

    
    def drawCharacter_(self, pos, code, hp, direction):
        x,y = pos
        img = self.assets.getCharacter(code)
        self.drawImageGrid(img, x, y)

        self.gridhpshower.updatepos(self.getGridPoint(x, y ,"bottomleft",transform = (0,-20)))
        self.gridhpshower.text = f"{hp.value}/{hp.max}"
        self.gridhpshower.draw(self.screen, self.font)

        gp = self.getGridPoint(x, y, "middle")
        ge = self.getGridPoint(x, y, "middle", transform = (direction[0]*20,direction[1]*20))
        
        pygame.draw.line(self.screen, (255,255,255), gp, ge)
        

    def drawCharacter(self, character):
        x,y = c.position.x, c.position.y
        dx, dy = c.direction.x, c.direction.y
        self.drawCharacter_((x,y), c.characterCode, c.hp, (dx,dy))
    
    def drawWorldEvents(self):
        for we in self.worldevents:
            text = we[0]
            pos = we[1]
            self.drawTextToGrid(text, pos, (0,10))

    def drawcharacters(self,characters):
        for c in characters:
            x,y = c.position.x, c.position.y
            dx, dy = c.direction.x, c.direction.y
            self.drawCharacter_((x,y), c.characterCode, c.hp, (dx,dy))

    def draw(self):
        self.drawBackground()

        for x in range(-5,5):
            for y in range(-5,5):
                self.drawColorGrid(x,y,(100,100,100))
        
        self.drawcharacters(self.characters)
        if self.observer != None: self.drawCharacter_((self.observer[0].x, self.observer[0].y), 123, self.observer[1], (self.observer[2].x, self.observer[2].y))
        self.drawWorldEvents()

        self.drawUI()

        if self.currentrequest == REQ_DIRECTION and self.directionchoosestate != None:
            pygame.draw.line(self.screen, (255,255,255), self.directionchoosestate, self.mousepos)
    


    # MAIN LOOP

    def loop(self):
        while self.run:
            self.mousepos = pygame.mouse.get_pos()
            self.events()
            self.cameracontrolloop(self.mousepos)

            if self.currentrequest == REQ_EVENTER:
                i = 0
                nbuttons = self.uitool.getButtons(len(self.requestinfo), "bottom-buttonsize", self.eventeronclick, transform=(0,-62))
                for nbutton in nbuttons:
                    nbutton.updateimage( self.assets.getEventer(self.requestinfo[i].name) )
                    i+=1
                self.eventerbuttons = nbuttons
                self.eventerbuttons.append( self.uitool.getButton(self.assets.get("CancelButton"), -1, "upright-buttonsize", self.eventeronclick) )
                self.currentrequest = REQ_EVENTER_MID
            elif self.currentrequest == REQ_CHARACTER_SELECTION:
                i = 0
                nbuttons = self.uitool.getButtons(len(self.requestinfo), "middle", self.characterchoiceonclick)
                for nbutton in nbuttons:
                    nbutton.updateimage(self.assets.getCharacter(self.requestinfo[i][0]))
                    i+=1
                self.characterchoicebuttons = nbuttons

                self.currentrequest = REQ_CHARACTER_SELECTION_MID
            

            self.screen.fill((0, 0, 0))
            self.draw()
            pygame.display.flip()