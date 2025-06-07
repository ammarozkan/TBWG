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
