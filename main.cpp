#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} Button;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

void renderButton(SDL_Renderer* renderer, Button* button) {
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}

void displayBoard() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* plateauWindow = SDL_CreateWindow("Plateau", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* plateauRenderer = SDL_CreateRenderer(plateauWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect exitButton = { 300, 400, 200, 50 };
    SDL_Color buttonColor = { 255, 0, 0, 255 };

    // Load title.bmp as a texture
    SDL_Surface* titleSurface = SDL_LoadBMP("data/title.bmp");
    if (titleSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", "title.bmp", SDL_GetError());
        return;
    }
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(plateauRenderer, titleSurface);
    SDL_Rect titleRect = { 260, 200, 120, 50 }; // Adjust position and size as needed

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_a) {
                    quit = 1; // Quit if 'a' key is pressed
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
                    y >= exitButton.y && y <= exitButton.y + exitButton.h) {
                    quit = 1; // Quit if exit button is clicked
                }
            }
        }

        SDL_SetRenderDrawColor(plateauRenderer, 255, 255, 255, 255);
        SDL_RenderClear(plateauRenderer);

        SDL_SetRenderDrawColor(plateauRenderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_RenderFillRect(plateauRenderer, &exitButton);

        // Render the title texture
        SDL_RenderCopy(plateauRenderer, titleTexture, NULL, &titleRect);

        SDL_RenderPresent(plateauRenderer);
    }

    // Cleanup
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyRenderer(plateauRenderer);
    SDL_DestroyWindow(plateauWindow);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture* titleTexture = loadTexture("data/welcome.bmp", renderer);
    SDL_Rect titleRect = { 200, 50, 400, 50 };

    Button playButton = { { 300, 200, 200, 50 }, loadTexture("data/play.bmp", renderer) };
    Button exitButton = { { 300, 300, 200, 50 }, loadTexture("data/exit.bmp", renderer) };

    if (!titleTexture || !playButton.texture || !exitButton.texture) {
        printf("Failed to load one or more textures!\n");
        return 1;
    }

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= playButton.rect.x && x <= playButton.rect.x + playButton.rect.w &&
                    y >= playButton.rect.y && y <= playButton.rect.y + playButton.rect.h) {
                    printf("Play button clicked!\n");
                    displayBoard();


                } else if (x >= exitButton.rect.x && x <= exitButton.rect.x + exitButton.rect.w &&
                           y >= exitButton.rect.y && y <= exitButton.rect.y + exitButton.rect.h) {
                    quit = 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        renderButton(renderer, &playButton);
        renderButton(renderer, &exitButton);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(playButton.texture);
    SDL_DestroyTexture(exitButton.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
