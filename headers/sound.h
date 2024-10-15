#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void createsound(SDL_Window *win, SDL_Renderer *ren, int loop, int song);

void Create_Sample(SDL_Window *win, SDL_Renderer *ren, int effect);

void create_effect(SDL_Window *win, SDL_Renderer *ren, int EFFECT);

#endif 