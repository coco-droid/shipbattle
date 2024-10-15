@echo off
if exist client.exe del client.exe
gcc -o client.exe    scripts\*.c   scripts\window\*.c  scripts\sockets\*.c test2.c -w -lwebsockets -lcjson -lws2_32 -liphlpapi -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lgdi32 -lSDL2_gfx -mconsole 2> logs.txt
client.exe