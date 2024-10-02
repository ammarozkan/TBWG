makelib:
	gcc src/* -I include -c
	mv *.o objs/
	ar rcs libtbwg.a objs/*.o
	mv *.a libs/

clean:
	rm objs/*