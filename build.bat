@echo off
if exist test.exe del test.exe
gcc -o test  test.c  -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lgdi32 -lSDL2_gfx -mconsole 2> logs.txt
test.exe