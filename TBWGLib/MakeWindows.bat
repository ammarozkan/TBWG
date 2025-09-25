echo off


mkdir outs
mkdir libs
mkdir objs


gcc src/essentials.c  -I include -c -o essentialslibo.o
gcc src/lists.c  -I include -c -o esslistslibo.o
gcc src/maths.c -I include -c -o essmathslibo.o
ar rcs libs/libtbwgessentials.a essentialslibo.o esslistslibo.o essmathslibo.o
del *.o


gcc src/* src/system/* -I include -c
move *.o objs/
ar rcs libtbwg.a objs/*.o
move *.a libs/
del objs/*.o

gcc -DTBWG_DEBUG src/* -I include -c
move *.o objs/
ar rcs libtbwgdebug.a objs/*.o
move *.a libs/
del objs/*.o


gcc tbwgnetsrc/* -I include -c
move *.o objs/
ar rcs libtbwgnet.a objs/*.o
move *.a libs/
del objs/*.o

gcc -DTBWG_DEBUG tbwgnetsrc/* -I include -c
move *.o objs/
ar rcs libtbwgnetdebug.a objs/*.o
move *.a libs/


gcc testmain.c -I include/ -L libs -ltbwgdebug -lm -o outs/testout


echo "press enter and test will start!"
pause
.\outs\testout.exe
pause