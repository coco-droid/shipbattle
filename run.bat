@echo off
if exist shipbattle.exe del shipbattle.exe
gcc -o shipbattle scripts\*.c scripts\window\*.c main.c -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lgdi32 -lSDL2_gfx -mconsole 2> logs.txt
shipbattle.exe
