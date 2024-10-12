@echo off
if exist shipbattle.exe del shipbattle.exe
gcc -o shipbattle scripts\*.c scripts\window\*.c scripts\sockets\*.c main.c -w -lwebsockets -lcjson -lws2_32 -liphlpapi -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lgdi32 -lSDL2_gfx -mconsole 2> logs.txt
shipbattle.exe
