struct List getAvailableCharacterInfos(struct Dimension* dimension)
{
	struct List ptsizedCharInfos = createList();

	ITERATE(dimension->characterList, charListElm_pure) {
		struct CharacterListElement* charListElm = (struct CharacterListElement*)charListElm_pure;

		if (!tbwgIsDefaultControllerInterface(charListElm->character->controllerInterface)) continue;


		struct TBWGConCharacterInformation inf = {.code = charListElm->character->b.code};
		struct TBWGConPtsizedCharacterInformationListElement elm = {.charinf.systematicPtr = charListElm->character, .charinf.inf = inf};
		addElement(&ptsizedCharInfos, &elm, sizeof(elm));
		printf("Empty character found on first dimension! Code:%u\n",elm.charinf.inf.code);
	}
	return ptsizedCharInfos;
}