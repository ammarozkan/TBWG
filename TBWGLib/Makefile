lib:
	gcc src/* -I include -c
	mv *.o objs/
	ar rcs libtbwg.a objs/*.o
	mv *.a libs/

clean:
	rm objs/*

test:
	echo test starts!
	gcc testmain.c -I include/ -L libs -ltbwg -o testout
	./testout
