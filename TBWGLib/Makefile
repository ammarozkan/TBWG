debuglib:
	gcc src/* -I include -DTBWG_DEBUG -c
	mv *.o objs/
	ar rcs libtbwgdebug.a objs/*.o
	mv *.a libs/


lib:
	gcc src/* tbwgnetsrc/* -I include -c
	mv *.o objs/
	ar rcs libtbwg.a objs/*.o
	mv *.a libs/

clean:
	rm objs/*

glassbreaker:
	rm -r objs
	rm -r libs
	rm testout


debugtest:
	echo debug test starts!
	gcc -DTBWG_DEBUG testmain.c -I include/ -L libs -ltbwgdebug -lm -o testout
	./testout
test:
	echo test starts!
	gcc testmain.c -I include/ -L libs -ltbwg -lm -o testout
	./testout
