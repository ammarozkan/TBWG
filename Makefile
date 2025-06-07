necessarydirs:
	@if [ ! -d outs ]; then mkdir outs; fi

serverdemo:
	make necessarydirs
	gcc tbwgserverdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwg -ltbwgnet -ltbwgessentials -lm -o outs/tbwgserverdemo.out
	gcc tbwgclientdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgnet -ltbwgessentials -lm -o outs/tbwgclientdemo.out

debugserverdemo:
	make necessarydirs
	gcc tbwgserverdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgdebug -ltbwgnetdebug -ltbwgessentials -lm -o outs/tbwgserverdemo.out
	gcc tbwgclientdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgnetdebug -ltbwgessentials -lm -o outs/tbwgclientdemo.out

glassbreaker:
	rm -r outs

completecompile:
	make necessarydirs
	cd TBWGLib
	make tbwgessentials lib
	cd ..
	make serverdemo