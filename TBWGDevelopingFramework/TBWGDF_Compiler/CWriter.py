
def getAsCFloat(number):
    if type(number) == int:
        return f"{number}.0f"
    else:
        return f"{number}f"