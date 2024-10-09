#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#define NUM_SLIDES 3

// Structure pour contenir les informations d'une diapositive
typedef struct {
    SDL_Texture* imageTexture;
    SDL_Rect imageRect;
    SDL_Texture* textTexture;
    SDL_Rect textRect;
} Slide;

// Fonction pour charger une texture depuis un fichier
SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return texture;
}

// Fonction pour créer une texture de texte
SDL_Texture* CreateTextTexture(const char* text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer, SDL_Rect* rect) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) {
        printf("TTF_RenderUTF8_Blended Error: %s\n", TTF_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface (Text) Error: %s\n", SDL_GetError());
    }
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1){
        printf("TTF_Init Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window* window = SDL_CreateWindow("Carrousel SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window){
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de la police
    TTF_Font* font = TTF_OpenFont("medias/font/Sora.ttf", 24); // Remplacez par le chemin de votre police
    if (!font){
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Configuration des diapositives
    const int numSlides = 3; // Nombre de diapositives
    Slide slides[numSlides];

    // Exemple de données, à adapter selon vos besoins
    const char* imagePaths[NUM_SLIDES] = {
        "medias/images/dialog_1i.png",
        "medias/images/dialog_2.jpeg",
        "medias/images/dialog_1i.png"
    };

    const char* texts[NUM_SLIDES] = {
        "Texte pour image 1",
        "Texte pour image 2",
        "Texte pour image 3"
    };

    // Configuration des diapositives
    for(int i = 0; i < NUM_SLIDES; i++) {
        slides[i].imageTexture = LoadTexture(imagePaths[i], renderer);
        if (!slides[i].imageTexture){
            // Gérer l'erreur de chargement
            // Nettoyage des ressources déjà chargées
            for(int j = 0; j < i; j++){
                SDL_DestroyTexture(slides[j].imageTexture);
                SDL_DestroyTexture(slides[j].textTexture);
            }
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        // Définir la position et la taille de l'image
        slides[i].imageRect.x = 100; // Exemple de position X
        slides[i].imageRect.y = 100; // Exemple de position Y
        slides[i].imageRect.w = 600; // Exemple de largeur
        slides[i].imageRect.h = 400; // Exemple de hauteur

        // Créer la texture de texte
        SDL_Color color = {255, 255, 255, 255}; // Blanc
        slides[i].textTexture = CreateTextTexture(texts[i], font, color, renderer, &slides[i].textRect);
        if (!slides[i].textTexture){
            // Gérer l'erreur de création du texte
            for(int j = 0; j <= i; j++){
                SDL_DestroyTexture(slides[j].imageTexture);
                if (j < i) SDL_DestroyTexture(slides[j].textTexture);
            }
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        // Positionner le texte au centre de l'image
        slides[i].textRect.x = slides[i].imageRect.x + (slides[i].imageRect.w - slides[i].textRect.w) / 2;
        slides[i].textRect.y = slides[i].imageRect.y + slides[i].imageRect.h - slides[i].textRect.h - 10; // 10 pixels du bas
    }

    bool running = true;
    SDL_Event event;
    int currentSlide = 0;
    Uint32 lastChangeTime = SDL_GetTicks();
    const Uint32 slideDuration = 3000; // 3 secondes par diapositive

    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }
            // Vous pouvez ajouter des contrôles pour changer de diapositive manuellement
        }

        // Changement automatique de diapositive
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastChangeTime >= slideDuration){
            currentSlide = (currentSlide + 1) % numSlides;
            lastChangeTime = currentTime;
        }

        // Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        // Rendre l'image actuelle
        SDL_RenderCopy(renderer, slides[currentSlide].imageTexture, NULL, &slides[currentSlide].imageRect);
        // Rendre le texte
        SDL_RenderCopy(renderer, slides[currentSlide].textTexture, NULL, &slides[currentSlide].textRect);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    for(int i = 0; i < numSlides; i++){
        SDL_DestroyTexture(slides[i].imageTexture);
        SDL_DestroyTexture(slides[i].textTexture);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
