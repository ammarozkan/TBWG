from __init__ import *


## ARGUMENTS

import sys
n = len(sys.argv)
widenet = False
ip = "127.0.0.1"
port = 5005
address = "google.com"

if n == 2:
    address = sys.argv[1]
    widenet = True
    print("WIDENET ",address)
elif n == 3:
    ip = sys.argv[1]
    port = int(sys.argv[2])

    widenet = False
else:
    address=input("ADRESS:")
    widenet=True

## ARGUMENTSEND



Player.initsoundlib()
assets = getTGFAssets("tgfassets/")

program(assets=assets, tbwgconenable=True, widenet=widenet, address=address, ip=ip, port=port, name="John")