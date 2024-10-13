#include "../headers/graphics.h"
#include "../headers/log.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#include "../headers/events.h"
#include <stdio.h>
#include <string.h>
SDL_Window *first_window = NULL;
SDL_Renderer *first_renderer = NULL;
SDL_Window *second_window = NULL;
SDL_Renderer *second_renderer = NULL;
void SetRoundedCorners(SDL_Window* window,int w,int h) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    if (!SDL_GetWindowWMInfo(window, &wmInfo)) {
        fprintf(stderr, "Failed to get window manager info: %s\n", SDL_GetError());
        return;
    }

    HWND hWnd = wmInfo.info.win.window;

    // Create a round rectangle region
    HRGN hRgn = CreateRoundRectRgn(0, 0, w, h, 50, 50);

    // Set the window region
    SetWindowRgn(hWnd, hRgn, TRUE);
}

SDL_Window* CreateRoundedWindow(const char *title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window) {
        SetRoundedCorners(window,w,h);
    }
    return window;
}

void OpenPlayWindow(SDL_Window* window1, SDL_Renderer* renderer1, SDL_Window** window2, SDL_Renderer** renderer2) {
    // Fermer la première fenêtre et le premier renderer
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window1);

    // Créer la nouvelle fenêtre
    *window2 = CreateRoundedWindow("Rounded Corner Window",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             870,
                                             600,
                                             SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    if (!*window2) {
        SDL_Log("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Créer le nouveau renderer
    *renderer2 = SDL_CreateRenderer(*window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer2) {
        SDL_Log("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window2);
        SDL_Quit();
        return;
    }

    // Définir la couleur de dessin sur noir (r, g, b, a) -> (0, 0, 0, 255)
    SDL_SetRenderDrawColor(*renderer2, 0, 0, 0, 255);

    // Effacer l'écran avec la couleur définie (noir)
    SDL_RenderClear(*renderer2);

    // Présenter le rendu (mettre à jour l'écran)
    SDL_RenderPresent(*renderer2);

    // Boucle d'événements pour maintenir la fenêtre ouverte
    bool running = true;
}

void OpenMenuWindow(SDL_Window* window1, SDL_Renderer* renderer1, SDL_Window** window2, SDL_Renderer** renderer2) {
    // Fermer la première fenêtre et le premier renderer
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window1);

    // Créer la nouvelle fenêtre
    *window2 = CreateRoundedWindow("Rounded Corner Window",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             600,
                                             600,
                                             SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    if (!*window2) {
        SDL_Log("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Créer le nouveau renderer
    *renderer2 = SDL_CreateRenderer(*window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer2) {
        SDL_Log("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window2);
        SDL_Quit();
        return;
    }

    // Définir la couleur de dessin sur noir (r, g, b, a) -> (0, 0, 0, 255)
    SDL_SetRenderDrawColor(*renderer2, 0, 0, 0, 255);

    // Effacer l'écran avec la couleur définie (noir)
    SDL_RenderClear(*renderer2);

    // Présenter le rendu (mettre à jour l'écran)
    SDL_RenderPresent(*renderer2);

    // Boucle d'événements pour maintenir la fenêtre ouverte
    bool running = true;
}


void CreateClickableElement(SDL_Renderer* renderer, int x, int y, int* w, int* h, const char* text, SDL_Color textColor, const char* imagePath, EventCallback callback, int fontSize) {
    // Load background image if provided
    InitLogFile("logs.txt");
    SDL_Surface* imageSurface = NULL;
    SDL_Texture* texture = NULL;
    int imageW = 0, imageH = 0;
    if (imagePath) {
        imageSurface = IMG_Load(imagePath);
        if (!imageSurface) {
            printf("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        imageW = imageSurface->w;
        imageH = imageSurface->h;
    }

    // If width or height is not set, use the size of the image
    if (*w == 0 && imageSurface) *w = imageW;
    if (*h == 0 && imageSurface) *h = imageH;

    // Render the text if provided
    SDL_Surface* textSurface = NULL;
    if (text) {
        TTF_Font* font = TTF_OpenFont("medias/font/Minercraftory.ttf", fontSize);
        if (!font) {
            printf("TTF_OpenFont: %s\n", TTF_GetError());
            SDL_FreeSurface(imageSurface);
            return;
        }
        textSurface = TTF_RenderText_Blended(font, text, textColor);
        if (!textSurface) {
            printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            SDL_FreeSurface(imageSurface);
            return;
        }
        TTF_CloseFont(font);

        // Get the dimensions of the text
        int textW = textSurface->w;
        int textH = textSurface->h;

        // Center the text within the image
        SDL_Rect textRect = {(*w - textW) /2.3, (*h - textH)/2.5, textW, textH};
        if (imageSurface) {
            SDL_BlitSurface(textSurface, NULL, imageSurface, &textRect);
        }
    }

    // Convert the combined surface to texture
    if (imageSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

    if (textSurface) {
        SDL_FreeSurface(textSurface);
    }

    SDL_Rect rect = {x, y, *w, *h};

    // Render the combined texture
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }

    // Créer et enregistrer la zone cliquable
    ClickableArea area = {
        .x = rect.x,
        .y = rect.y,
        .width = rect.w,
        .height = rect.h,
        .onClick = callback,
        .onHover = NULL,
        .onDragStart = NULL,
        .onDragEnd = NULL,
        .onDragMove = NULL
    };

    RegisterClickableArea(area);
}
void RenderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, int fontSize, int maxWidth) {
    // Load the font
    TTF_Font* font = TTF_OpenFont("medias/font/Sora.ttf", fontSize);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    // Variables for managing the text rendering
    char buffer[1024];
    const char* word;
    char line[1024] = "";
    int lineHeight = TTF_FontHeight(font);
    int offsetY = 0;

    // Split the input text into words
    strncpy(buffer, text, sizeof(buffer) - 1);
    word = strtok(buffer, " ");

    // Loop over words and construct lines
    while (word != NULL) {
        char testLine[1024];
        snprintf(testLine, sizeof(testLine), "%s %s", line, word);

        // Render test surface to check the width of the line
        SDL_Surface* testSurface = TTF_RenderText_Blended(font, testLine, color);
        if (testSurface == NULL) {
            printf("Failed to render text: %s\n", TTF_GetError());
            break;
        }

        if (testSurface->w > maxWidth) {
            // Render the current line as it fits
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, line, color);
            if (!textSurface) {
                printf("Failed to render text: %s\n", TTF_GetError());
                break;
            }

            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            // Get the dimensions of the text
            int textW, textH;
            SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);

            // Set the text position
            SDL_Rect textRect = {x, y + offsetY, textW, textH};

            // Render the text
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);

            // Move to the next line
            offsetY += lineHeight;
            strncpy(line, word, sizeof(line) - 1);
        } else {
            // Add word to the current line
            strncpy(line, testLine, sizeof(line) - 1);
        }

        // Clean up the test surface
        SDL_FreeSurface(testSurface);

        // Move to the next word
        word = strtok(NULL, " ");
    }

    // Render the last line
    if (strlen(line) > 0) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, line, color);
        if (!textSurface) {
            printf("Failed to render text: %s\n", TTF_GetError());
            return;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        // Get the dimensions of the text
        int textW, textH;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);

        // Set the text position
        SDL_Rect textRect = {x, y + offsetY, textW, textH};

        // Render the text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    // Clean up
    TTF_CloseFont(font);
}