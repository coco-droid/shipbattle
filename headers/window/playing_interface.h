#ifndef PLAYING_INTERFACE.h
#define PLAYING_INTERFACE.h 
void FireAtCell(int boat_pos[10][10], int hint[10][10], int x, int y);
void PlayingInterface(SDL_Window* Window, SDL_Renderer* Renderer);
#endif