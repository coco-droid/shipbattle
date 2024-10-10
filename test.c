#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define NUM_SLIDES 4

// Déclaration des variables globales
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Mix_Music *music = NULL;

// Fonction pour initialiser et jouer de la musique
void playMusic(const char *fileName, int duree) {
    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        fprintf(stderr, "Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        return;
    }

    // Chargement de la musique avec le nom de fichier passé en argument
    music = Mix_LoadMUS(fileName);
    if (music == NULL) {
        fprintf(stderr, "Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        Mix_CloseAudio();
        return;
    }

    // Réglage du volume et lecture de la musique
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    if (Mix_PlayMusic(music, -1) == -1) {
        fprintf(stderr, "Erreur Mix_PlayMusic: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        return;
    }

    // Boucle d'attente pour écouter la musique
    SDL_Delay(duree);

    // Nettoyage
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

// Structure pour contenir les informations d'une diapositive
typedef struct {
    SDL_Texture *imageTexture;
    SDL_Rect imageRect;
    SDL_Texture *textTexture;
    SDL_Rect textRect;
} Slide;

// Fonction pour charger une texture depuis un fichier
SDL_Texture *LoadTexture(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return texture;
}

// Fonction pour créer une texture de texte
SDL_Texture *CreateTextTexture(const char *text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, SDL_Rect *rect) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) {
        printf("TTF_RenderUTF8_Blended Error: %s\n", TTF_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface (Text) Error: %s\n", SDL_GetError());
    }
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    window = SDL_CreateWindow("Carrousel SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Ajoute ici le code pour utiliser les textures ou lire de la musique...

    // Nettoyage avant la sortie du programme
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
