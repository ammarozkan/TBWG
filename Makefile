outsdir:
	mkdir outs

serverdemo:
	gcc tbwgserverdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwg -ltbwgnet -ltbwgessentials -lm -o outs/tbwgserverdemo.out
	gcc tbwgclientdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgnet -ltbwgessentials -o outs/tbwgclientdemo.out

debugserverdemo:
	gcc tbwgserverdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgdebug -ltbwgnetdebug -ltbwgessentials -lm -o outs/tbwgserverdemo.out
	gcc tbwgclientdemo.c -I TBWGLib/include/ -L TBWGLib/libs/ -ltbwgnetdebug -ltbwgessentials -o outs/tbwgclientdemo.out

glassbreaker:
	rm -r outs