necessarydirs:
	@if [ ! -d ../bin ]; then mkdir ../bin; fi


tgf:
	make necessarydirs
	gcc main.c -I ../TBWGLib/include/ -L ../TBWGLib/libs/ -ltbwg -ltbwgnet -ltbwgessentials -lm -o ../bin/tbwg_thegreatfighters.out

tgf_debug:
	make necessarydirs
	gcc main.c -I ../TBWGLib/include/ -L ../TBWGLib/libs/ -ltbwgdebug -ltbwgnetdebug -ltbwgessentials -lm -o ../bin/tbwg_thegreatfighters_debug.out