necessarydirs:
	@if [ ! -d objs ]; then mkdir objs; fi
	@if [ ! -d libs ]; then mkdir libs; fi
	@if [ ! -d outs ]; then mkdir outs; fi

tbwgnetlib:
	make cleanobjs; make necessarydirs
	gcc tbwgnetsrc/* -I include -c
	mv *.o objs/
	ar rcs libtbwgnet.a objs/*.o
	mv *.a libs/

tbwglib:
	make cleanobjs; make necessarydirs
	gcc src/* -I include -c
	mv *.o objs/
	ar rcs libtbwg.a objs/*.o
	mv *.a libs/

tbwgnetdebuglib:
	echo "Net Debug Lib Compile!"
	make cleanobjs; make necessarydirs
	gcc -DTBWG_DEBUG tbwgnetsrc/* -I include -c
	mv *.o objs/
	ar rcs libtbwgnetdebug.a objs/*.o
	mv *.a libs/

tbwgdebuglib:
	make cleanobjs; make necessarydirs
	gcc -DTBWG_DEBUG src/* -I include -c
	mv *.o objs/
	ar rcs libtbwgdebug.a objs/*.o
	mv *.a libs/

tbwgessentialslib:
	echo "Essential Lib"
	make necessarydirs
	gcc src/essentials.c  -I include -c -o essentialslibo.o
	gcc src/lists.c  -I include -c -o esslistslibo.o
	gcc src/maths.c -I include -c -o essmathslibo.o
	ar rcs libs/libtbwgessentials.a essentialslibo.o esslistslibo.o essmathslibo.o
	rm essentialslibo.o esslistslibo.o essmathslibo.o


debug:
	make tbwgnetdebuglib
	make tbwgdebuglib

lib:
	make tbwglib
	make tbwgnetlib

cleanobjs:
	@if ls *.o 1> /dev/null 2>&1; then echo "Found object files on base, removing."; rm *.o; else echo "*.o Clean!"; fi

	@if ls objs/* 1> /dev/null 2>&1; then echo "Found files on objs, removing."; rm objs/*; else echo "objs/ Clean!"; fi

	@if ls objs 1> /dev/null 2>&1; then echo "Found objs directory, removing"; rmdir objs; else echo "objs Clean!"; fi

clean:
	make cleanobjs

glassbreaker:
	rm -r objs
	rm -r libs
	rm -r outs


debugtest:
	echo debug test starts!
	gcc -DTBWG_DEBUG testmain.c -I include/ -L libs -ltbwgdebug -ltbwgnet -lm -o outs/testout
	./testout
test:
	echo test starts!
	gcc testmain.c -I include/ -L libs -ltbwg -ltbwgnet -lm -o outs/testout
	./

nettest:
	make tbwgnetlib
	echo nettest starts!
	gcc tbwgcontest.c -I include/ -L libs -ltbwgnet -ltbwgessentials -o outs/tbwgcontest.out

	./outs/tbwgcontest.out

debugnettest:
	make tbwgnetdebuglib
	echo ""
	echo ""
	echo "-------------nettest starts!"
	echo ""
	gcc tbwgcontest.c -I include/ -L libs -ltbwgnetdebug -ltbwgessentials -o outs/tbwgcontest.out

	./outs/tbwgcontest.out