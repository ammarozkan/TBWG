debuglib:
	gcc src/* -I include -DTBWG_DEBUG -c
	mv *.o objs/
	ar rcs libtbwgdebug.a objs/*.o
	mv *.a libs/


lib:
	gcc src/* -I include -c
	mv *.o objs/
	ar rcs libtbwg.a objs/*.o
	mv *.a libs/

clean:
	rm objs/*



debugtest:
	echo debug test starts!
	gcc -DTBWG_DEBUG testmain.c -I include/ -L libs -ltbwgdebug -lm -o testout
	./testout
test:
	echo test starts!
	gcc testmain.c -I include/ -L libs -ltbwg -lm -o testout
	./testout
