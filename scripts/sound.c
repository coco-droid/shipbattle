#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../headers/sound.h"

void SDL_ExitWithError(message);

void createsound(SDL_Window *win, SDL_Renderer *ren, int loop, int song)
{
	 if(SDL_Init(SDL_INIT_AUDIO) !=0)
        {
           SDL_ExitWithError("impossible d'initialiser SDL_mixer");
        }

	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2 ,1024) != 0)
		{
			SDL_ExitWithError("impossible de charger l'audio");
		}

		Mix_Music *musiqueMP3 = NULL;
		char * son;
		switch(song)
		{
			case 1:
			son = "medias/SOUND/menu_background.mp3";
			break;

			case 2:
			son = "medias/SOUND/round.mp3";
			break;
			case 3: 
			son = "medias/SOUND/win_effect.mp3";
			break;
			case 4:
			son = "medias/SOUND/Lose_effect_1.mp3";
			break;
			case 5:
			son = "medias/SOUND/almost dead";
			break;
			case 6:
			son = "medias/SOUND/dead.mp3";
			break;
		}
		musiqueMP3 = Mix_LoadMUS(son);
	if (musiqueMP3 == NULL)
		{
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_ExitWithError("impossible de charger le son");
		}

		Mix_VolumeMusic(MIX_MAX_VOLUME/2);

	if(Mix_PlayMusic(musiqueMP3, loop) != 0)
		{
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			Mix_FreeMusic(musiqueMP3);
			Mix_CloseAudio();
			SDL_ExitWithError("impossible de jouer le son");
		}
		
		
}

void SDL_ExitWithError(message);

void Create_Sample(SDL_Window *win, SDL_Renderer *ren, int effect)
{
	 if(SDL_Init(SDL_INIT_AUDIO) !=0)
        {
           SDL_ExitWithError("impossible d'initialiser SDL_mixer");
        }

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) != 0)
		{
			SDL_ExitWithError("impossible de charger l'audio");
		}

		Mix_Chunk *sample = NULL;
		char *feedback, feedback1, feedback2;
		switch(effect)
		{
			case 1:
			feedback ="medias/SOUND/beep.wav";
			break;

			case 2:
			feedback = "medias/SOUND/clic(128k).wav";
			break;
			case 3:
			feedback = "medias/SOUND/Lose_effect_1_1.wav";
			break;
			
			case 5:
			feedback = "medias/SOUND/round started.wav";
			break;
			case 6:
			feedback = "medias/SOUND/win_effect.wav";
			break;
			
			case 8:
			feedback = "medias/SOUND/health.wav";
			break;
		}
		sample = Mix_LoadWAV(feedback);

		if (sample == NULL)
		{
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_ExitWithError("impossible de charger le sample");
		}

		Mix_VolumeChunk(sample, MIX_MAX_VOLUME );
		

		if (Mix_PlayChannel(-1, sample, 0) == -1)
		 {
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_ExitWithError("impossible de jouer le sample");
		}
		



}

void create_effect(SDL_Window *win, SDL_Renderer *ren, int EFFECT)
{
	 if(SDL_Init(SDL_INIT_AUDIO) !=0)
        {
           SDL_ExitWithError("impossible d'initialiser SDL_mixer");
        }

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) != 0)
		{
			SDL_ExitWithError("impossible de charger l'audio");
		}

		Mix_Chunk *sunk_sample = NULL;
		Mix_Chunk *miss_sample = NULL;
		Mix_Chunk *touchdown_sample = NULL;
		char *feedback1;
		char *feedback2;
		char *feedback3;

		switch(EFFECT)
		{

			case 1:
			feedback1 = "medias/SOUND/boom_1.wav";
			sunk_sample = Mix_LoadWAV(feedback1);
			Mix_VolumeChunk(sunk_sample, MIX_MAX_VOLUME );

			if (sunk_sample == NULL)
			{
				SDL_DestroyRenderer(ren);
				SDL_DestroyWindow(win);
				SDL_ExitWithError("impossible de charger l'effet toucher");
			}

			if (Mix_PlayChannel(-1, sunk_sample, 0) == -1)
			{
				SDL_DestroyRenderer(ren);
				SDL_DestroyWindow(win);
				SDL_ExitWithError("impossible de jouer l'effet toucher");
		 	}
		 	while(Mix_Playing(-1) != 0)
				{
					SDL_Delay(100);
				}
				break;

			case 4:
			feedback2 = "medias/SOUND/plouf.wav";
			miss_sample = Mix_LoadWAV(feedback2);
			Mix_VolumeChunk(miss_sample, MIX_MAX_VOLUME );


			if (miss_sample == NULL)
			{
				SDL_DestroyRenderer(ren);
				SDL_DestroyWindow(win);
				SDL_ExitWithError("impossible de charger l'effet rater");
			}

			if (Mix_PlayChannel(-1, miss_sample, 0) == -1)
		 	{
				SDL_DestroyRenderer(ren);
				SDL_DestroyWindow(win);
				SDL_ExitWithError("impossible de jouer l'effet rater");
			}
				while(Mix_Playing(-1) != 0)
				{
					SDL_Delay(100);
				}
				break;

			case 7:
			feedback3 = "medias/SOUND/touchdown.wav";
			touchdown_sample = Mix_LoadWAV(feedback3);
			Mix_VolumeChunk(touchdown_sample, MIX_MAX_VOLUME);
			Mix_PlayChannel(-1, touchdown_sample, 0);
		 	
		}

		
}

void SDL_ExitWithError(const char *message)
{
	SDL_Log("ERREUR: %s->%s\n", message, SDL_GetError);
	SDL_Quit();
	exit(EXIT_FAILURE);
}
