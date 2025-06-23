class Camera:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.z = 0

        self.p = 0.5

        self.scalex = 1
        self.scaley = 1
        self.scalez = 1

        self.grp = self.getRelativePos
        self.grl = self.getRelativeLength
        self.ugrp = self.getUnRelativePos
        self.ugrl = self.getUnRelativeLength
    
    def getRelativePos(self, x, y, z):
        return self.scalex*(x-self.x), -self.scaley*(y - self.y) + self.scalez*(z*self.p - self.z)
    def getRelativeLength(self, x, y, z):
        return self.scalex*x, self.scaley*y + self.scalez*z*self.p
    def getUnRelativePos(self, x, z):
        return (x/self.scalex + self.x), (z/self.scalez + self.z)/self.p
    def getUnRelativeLength(self, x, z):
        return x/self.scalex, z/(self.scalez*self.p)
    def closer(self):
        self.x*=self.scalex
        self.y*=self.scaley
        self.z*=self.scalez
        self.scalex += 0.1
        self.scaley += 0.1
        self.scalez += 0.1
        self.x/=self.scalex
        self.y/=self.scaley
        self.z/=self.scalez
    def further(self):
        self.x*=self.scalex
        self.y*=self.scaley
        self.z*=self.scalez
        self.scalex -= 0.1
        self.scaley -= 0.1
        self.scalez -= 0.1
        self.x/=self.scalex
        self.y/=self.scaley
        self.z/=self.scalez
    
    def move(self, x, z):
        self.x += x
        self.z += z*self.p
