


iVector tgfModsGetUniquePosition()
{
    static unsigned int i = 0;
    iVector result;

    if (i == 0) result = getiVector(0,0);
    else if (i == 1) result = getiVector(2,2);
    else if (i == 2) result = getiVector(5,2);
    else if (i == 3) result = getiVector(1,-5);
    else if (i == 4) result = getiVector(-6,5);
    else if (i == 5) result = getiVector(-4,3);

    i+=1;
    return result;
}

struct Character* tgfModsGetRandomCharacter(struct Dimension* dim, iVector pos)
{
    unsigned int ch = rand()%TGF_MODS_CHARACTER_COUNT;
    return getCharacterByOrder(ch, dim, pos);
}


void tgfModsAddRandomCharacters(unsigned int count)
{
    for (unsigned int i = 0 ; i < count ; i += 1) {
        tbwgAddCharacter(tgfModsGetRandomCharacter(tbwgGetFirstDimension(),tgfModsGetUniquePosition()));
    }
}

void tgfModsWorldGeneration(unsigned int charCount)
{
    tgfModsAddRandomCharacters(charCount);
}