import pygame

def initsoundlib():
    pygame.mixer.init()

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

def getEventerPanelDefault(uitool, eventers, onclickfunc, viewerManager):
    i = 0
    nbuttons = uitool.getButtons(len(eventers), "bottom", onclickfunc, transform=(0,-92))
    for nbutton in nbuttons:
        img, getHovering = viewerManager.GetEventerView(eventers[i].eventerCode)
        nbutton.updateimage( img )
        nbutton.hoveringCode = f"[{eventers[i].energy} E | {eventers[i].spellenergy} SE] {eventers[i].costs} "+getHovering(eventers[i])
        i+=1
    img, getHovering = viewerManager.GetButtonView("CancelButton")
    nbuttons.append( uitool.getButton(img, -1, "Cancel", "upright", onclickfunc, transform = (-30,+30)) )
    return Panel (nbuttons)

def getCharacterSelectionPanelDefault(uitool, characters, onclickfunc, viewerManager):
    i = 0
    nbuttons = uitool.getButtons(len(characters), "middle", onclickfunc)
    for nbutton in nbuttons:
        img, getHovering = viewerManager.GetCharacterView(characters[i][0])
        nbutton.updateimage(img)
        nbutton.hoveringCode = getHovering(characters[i])
        i+=1
    return Panel(nbuttons)

class TBWGPyGamePlayer:
    def __init__(self, ScreenWidth, ScreenHeight, viewerManager, gridsize = 50):
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

        assets = viewerManager.assets
        self.assets = viewerManager.assets
        self.viewerManager = viewerManager

        self.uitool = UITool(ScreenWidth, ScreenHeight,assets.get("DefaultButton"), self.font)

        self.buttons = []
        self.buttons.append( self.uitool.getButton(assets.get("ClearWorldEvents"), 0, "clearworldevents", "bottomleft", self.clearWorldEvents, transform=(32,-32)) )

        self.eventerpanel = Panel()
        self.characterchoicepanel = Panel()
        self.effectspanel = Panel()

        self.hoveringText = ""

        self.worldevents = []
        self.worldevents_log = {}
        self.worldeventboxsize = 45
        
        self.restuses = self.uitool.getIconedTexts(5, 40, 20, self.assets.get("DefaultRestUses"), "bottom", transform=(0,-40))
        self.restuses[0].updateimage(self.assets.get("RestUses_classic"))
        self.restuses[0].hoveringCode = "Classic use. Everything."
        self.restuses[1].updateimage(self.assets.get("RestUses_armMove"))
        self.restuses[1].hoveringCode = "Arm movement."
        self.restuses[2].updateimage(self.assets.get("RestUses_handMove"))
        self.restuses[2].hoveringCode = "Hand movement."
        self.restuses[3].updateimage(self.assets.get("RestUses_movement"))
        self.restuses[3].hoveringCode = "Movement. Legs."
        self.restuses[4].updateimage(self.assets.get("RestUses_thought"))
        self.restuses[4].hoveringCode = "Thought. Executing via thinking."


        self.energy = (0,0)
        self.spellenergy = (0,0)

        self.gridhpshower = IconedText(assets.get("HealthIcon"), "0", (0,0), (20,20), "hpshower", self.font)
        self.effectshower = IconedText(assets.get("DefaultEffectIcon"), "0", (0,0), (50,50), "effectsshower", self.font)

        self.directionchoosestate = None

        self.background = self.assets.getBackground("default")

        self.shifting = False
    
    def updateBackground(self):
        self.background = pygame.transform.scale(self.background, (self.screenWidth, self.screenHeight))
        rect = pygame.Rect(0, 0, self.screenWidth,self.screenHeight)
    
    def changeBackground(self, image):
        self.background = image
        self.updateBackground()

    def addWorldEvent(self, we):
        if we[0][:4] == "SND_":
            self.assets.getSound(we[0]).play()
        key = str(we[1])
        if key not in self.worldevents_log: self.worldevents_log[key] = 1
        else: self.worldevents_log[key] += 1

        size = self.worldeventboxsize

        worldeventspr = self.assets.getWorldEvent(we[0])
        worldeventspr = pygame.transform.scale(worldeventspr, (size,size))

        self.worldevents.append((we[0], we[1], worldeventspr, self.worldevents_log[key]))
        print("APPENDMENT:",self.worldevents)
    
    def clearWorldEvents(self, code):
        self.worldevents = []
        self.worldevents_log = {}
        print("World events cleared.")
    
    def setRestUses(self, uses):
        for i in range(0,len(uses)):
            self.restuses[i].text = str(uses[i])
    
    def request(self, reqtype, info = None):
        if self.currentrequest != REQ_NOTHING: return REQ_BUSY

        if reqtype == REQ_EVENTER: self.assets.getSound("EventerRequest").play()
        elif reqtype == REQ_CHARACTER_SELECTION: self.assets.getSound("CharacterSelection").play()
        else: self.assets.getSound("OtherRequests").play()

        self.currentrequest = reqtype
        self.requestinfo = info
        while self.currentrequest != REQ_NOTHING: pass
        return self.lastrequest_answer
    
    def answerrequest(self, answer):
        self.lastrequest_answer = answer
        self.currentrequest = REQ_NOTHING
        return

    def getGridRect(self, x, z):
        x,z = x*self.gridsize, z*self.gridsize
        
        px, pz, sx, sz = x+1,z+1,self.gridsize-2,self.gridsize-2
        px, pz = self.camera.grp(px, 0, pz)
        sx, sz = self.camera.grl(sx, 0, sz)
        return (px,pz,sx,sz)
    
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
        print(gx,gy)
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
            if event.key == pygame.K_LSHIFT: self.shifting = True
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LSHIFT: self.shifting = False
        if event.type == pygame.QUIT:
            self.run = False
    
    def events(self):
        for event in pygame.event.get():
            self.event(event)
    
    def eventeronclick(self, id):
        if self.currentrequest != REQ_EVENTER_MID: return
        if id == -1: 
            self.assets.getSound("EndTurn").play()
            self.answerrequest(None)
        self.answerrequest(id)
        self.eventerpanel = Panel()
    
    def characterchoiceonclick(self, id):
        self.answerrequest(id)
        self.characterchoicepanel = Panel()
    
    def UIClickHandler(self, event):
        event = event
        
        if event == None: return event
        event = self.eventerpanel.eventcontrol(event)
        event = self.characterchoicepanel.eventcontrol(event)
        for button in self.buttons:
            event = button.eventcontrol(event)
        return event
    
    def GridClick(self, x, y):
        if self.currentrequest == REQ_POSITION:
            print("Answering request ",(x,y))
            self.answerrequest((x,y))
        
    def gethovering(self, mousepos):
        for panel in [self.eventerpanel, self.characterchoicepanel, self.effectspanel]:
            h = panel.gethovering(mousepos)
            if h != None: return h
        for r in self.restuses:
            h = r.gethovering(mousepos)
            if h != None: return h
        return None
    

    # DRAW

    def drawBackground(self):
        self.screen.blit(self.background,pygame.Rect(0,0,self.screenWidth,self.screenHeight))
    
    def drawTestGrid(self, x, y):
        rect = pygame.Rect(*(self.getGridRect(x,y)))
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
    
    
    def drawEntityImg(self, img, x, z, length, width):
        px, py, gsx, gsy = self.getGridRect(x,z)
        sx, sy = self.camera.grl(width, length, 0)
        px = px + gsx/2 - sx/2
        py = py - sy + gsy/2

        img = pygame.transform.scale(img, (sx, sy))
        rect = pygame.Rect(px,py,sx,sy)
        self.screen.blit(img,rect)
    
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
        self.effectspanel.elements = []
        r = []
        for ef in self.oneffects:
            img,getHovering = self.viewerManager.GetEffectView(str(ef.code))
            htext = getHovering(ef)
            r.append( IconedText(img, "", (30,i*50+30), (50, 50), htext, self.font) )
            i += 1
        self.effectspanel.addElements(r)
        self.effectspanel.draw(self.screen)
    
    def drawUI(self):
        self.eventerpanel.draw(self.screen)
        self.characterchoicepanel.draw(self.screen)
        for button in self.buttons:
            button.draw(self.screen)
        if self.currentrequest == REQ_EVENTER_MID: 
            for r in self.restuses: r.draw(self.screen)
        self.drawReq()

        self.drawBars()
        self.drawEffects()

    
    def drawCharacter_(self, pos, code, hp, direction):
        x,y = pos
        img = self.assets.getCharacter(code)
        self.drawEntityImg(img, pos[0], pos[1], 40, 20)

        self.gridhpshower.updatepos(self.getGridPoint(x, y ,"bottomleft",transform = (0,-20)))
        self.gridhpshower.text = f"{hp.value}/{hp.max}"
        self.gridhpshower.draw(self.screen)

        gp = self.getGridPoint(x, y, "middle")
        ge = self.getGridPoint(x, y, "middle", transform = (direction[0]*20,direction[1]*20))
        
        pygame.draw.line(self.screen, (255,255,255), gp, ge)
        

    def drawCharacter(self, c):
        x,y = c.position.x, c.position.y
        dx, dy = c.direction.x, c.direction.y
        self.drawCharacter_((x,y), c.characterCode, c.hp, (dx,dy))
    
    def drawWorldEvents(self):
        for we in self.worldevents:
            name, pos, spr, indice = we
            rect = self.getGridRect(pos[0],pos[1])
            sizex, sizey = self.camera.grl(self.worldeventboxsize, self.worldeventboxsize, 0)
            rect = (rect[0]+self.worldeventboxsize*indice, rect[1], sizex, sizey)
            self.screen.blit(we[2],rect)

    def drawcharacters(self, characters):
        #if self.observer != None: characters.append(self.observer) # adding the observer (managing character) to drawing list
        #for c in sorted(characters, key = lambda chr : chr.position.y if type(chr) != tuple else chr[0].y):
        if self.observer != None: 
            c = self.observer
            self.drawCharacter_((c[0].x, c[0].y), 123, c[1], (c[2].x, c[2].y))
        for c in characters:
            x,y = c.position.x, c.position.y
            dx, dy = c.direction.x, c.direction.y
            self.drawCharacter_((x,y), c.characterCode, c.hp, (dx,dy))
    
    def drawHoveringText(self):
        if not self.shifting: return
        x,y = self.mousepos
        text = self.hoveringText
        maxLength = 30
        lines = int(len(text)/maxLength)+1
        boxrect = (x+3,y,30*15,lines*30)

        xfix = 0
        yfix = 0
        if boxrect[0] + boxrect[2] > self.screenWidth:
            xfix = self.screenWidth - boxrect[0] - boxrect[2]
        if boxrect[1] + boxrect[3] > self.screenHeight:
            yfix = self.screenHeight - boxrect[1] - boxrect[3]
        
        if len(text) != 0: pygame.draw.rect(self.screen, (0,0,0), (boxrect[0]+xfix, boxrect[1]+yfix, boxrect[2], boxrect[3]))
        for i in range(0,lines):
            self.drawText(text[i*maxLength:(i+1)*maxLength],(x+xfix,y+i*30+yfix))

    def draw(self):
        self.drawBackground()

        for x in range(-5,5):
            for y in range(-5,5):
                pass
                self.drawColorGrid(x,y,(100,100,100))
        
        self.drawcharacters(self.characters)
        self.drawWorldEvents()

        self.drawUI()

        if self.currentrequest == REQ_DIRECTION and self.directionchoosestate != None:
            pygame.draw.line(self.screen, (255,255,255), self.directionchoosestate, self.mousepos)
        self.drawHoveringText()
    
    def updateHoveringInformation(self):
        hovering = self.gethovering(self.mousepos)
        if hovering == None: self.hoveringText = "";return
        self.hoveringText = hovering
    


    # MAIN LOOP

    def loop(self):
        while self.run:
            self.mousepos = pygame.mouse.get_pos()
            self.events()
            self.cameracontrolloop(self.mousepos)

            if self.currentrequest == REQ_EVENTER:
                self.eventerpanel = getEventerPanelDefault(self.uitool, self.requestinfo, self.eventeronclick, self.viewerManager)
                self.currentrequest = REQ_EVENTER_MID
            elif self.currentrequest == REQ_CHARACTER_SELECTION:
                self.characterchoicepanel = getCharacterSelectionPanelDefault(self.uitool, self.requestinfo, self.characterchoiceonclick, self.viewerManager)

                self.currentrequest = REQ_CHARACTER_SELECTION_MID
            

            self.screen.fill((0, 0, 0))
            self.draw()
            self.updateHoveringInformation()
            pygame.display.flip()