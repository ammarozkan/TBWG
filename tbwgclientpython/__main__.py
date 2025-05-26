import tbwgcon1

client = tbwgcon1.TBWGConClient("192.168.1.36",5005)


def characterselector(chrs):
    for c in chrs:
        print(f"{c}")
    return 0


client.setname("John")
client.setcharacterselector(characterselector)
if not client.connect():
    print("Some error happened while connecting")