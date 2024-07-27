#include "../headers/graphics.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#include <stdio.h>

void SetRoundedCorners(SDL_Window* window) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    if (!SDL_GetWindowWMInfo(window, &wmInfo)) {
        fprintf(stderr, "Failed to get window manager info: %s\n", SDL_GetError());
        return;
    }

    HWND hWnd = wmInfo.info.win.window;

    // Create a round rectangle region
    HRGN hRgn = CreateRoundRectRgn(0, 0, 600, 600, 50, 50);

    // Set the window region
    SetWindowRgn(hWnd, hRgn, TRUE);
}

SDL_Window* CreateRoundedWindow(const char *title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window) {
        SetRoundedCorners(window);
    }
    return window;
}

void CreateClickableElement(SDL_Renderer* renderer, int x, int y, int w, int h, const char* text, SDL_Color textColor, const char* imagePath, void (*callback)(void)) {
    SDL_Rect rect = {x, y, w, h};

    // Load background image if provided
    SDL_Texture *texture = NULL;
    if (imagePath) {
        SDL_Surface *surface = IMG_Load(imagePath);
        if (!surface) {
            printf("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Render the background texture or a filled rectangle
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Default color (red)
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render the text if provided
    if (text) {
        TTF_Font* font = TTF_OpenFont("medias/font/Minecraftory.ttf", 24);
        if (!font) {
            printf("TTF_OpenFont: %s\n", TTF_GetError());
            return;
        }
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, textColor);
        if (!textSurface) {
            printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            printf("CreateTextureFromSurface: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            TTF_CloseFont(font);
            return;
        }

        // Center the text within the element
        int textW = 0, textH = 0;
        TTF_SizeText(font, text, &textW, &textH);
        SDL_Rect textRect = {x + (w - textW) / 2, y + (h - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
    }

    // Handle events and call the callback if the element is clicked
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= x && mouseY >= y && mouseX <= x + w && mouseY <= y + h) {
                callback();
            }
        }
    }
}
