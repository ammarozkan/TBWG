necessarydirs:
	@if [ ! -d ../outs ]; then mkdir ../outs; fi


tgf:
	make necessarydirs
	gcc main.c -I ../TBWGLib/include/ -L ../TBWGLib/libs/ -ltbwg -ltbwgnet -ltbwgessentials -lm -o ../outs/tbwg_thegreatfighters.out

tgf_debug:
	make necessarydirs
	gcc main.c -I ../TBWGLib/include/ -L ../TBWGLib/libs/ -ltbwgdebug -ltbwgnetdebug -ltbwgessentials -lm -o ../outs/tbwg_thegreatfighters_debug.out