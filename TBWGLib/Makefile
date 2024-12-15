necessarydirs:
	@if [ ! -d objs ]; then mkdir objs; fi
	@if [ ! -d libs ]; then mkdir libs; fi

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
	make cleanobjs; make necessarydirs
	gcc tbwgnetsrc/* -I include -DTBWG_DEBUG -c
	mv *.o objs/
	ar rcs libtbwgnetdebug.a objs/*.o
	mv *.a libs/

tbwgdebuglib:
	make cleanobjs; make necessarydirs
	gcc src/* -I include -DTBWG_DEBUG -c
	mv *.o objs/
	ar rcs libtbwgdebug.a objs/*.o
	mv *.a libs/

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
	rm testout


debugtest:
	echo debug test starts!
	gcc -DTBWG_DEBUG testmain.c -I include/ -L libs -ltbwgdebug -ltbwgnet -lm -o testout
	./testout
test:
	echo test starts!
	gcc testmain.c -I include/ -L libs -ltbwg -ltbwgnet -lm -o testout
	./testout
