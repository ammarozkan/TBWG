from __init__ import *

Player.initsoundlib()
assets = getTGFAssets("tgfassets/")

program(assets=assets, tbwgconenable=True, ip="192.168.1.37", port=5005, name="John")