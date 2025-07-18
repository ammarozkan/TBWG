del ..\bin\tbwgserver.exe
gcc main.c -I ../TBWGLib/include/ -L ../TBWGLib/libs/ -ltbwgdebug -ltbwgnetdebug -ltbwgessentials -lws2_32 -lm -o ../bin/tbwgserver
echo OK
pause
.\..\bin\tbwgserver.exe --rounds 1024
pause