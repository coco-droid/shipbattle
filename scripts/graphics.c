#include "../headers/graphics.h"
#include "../headers/log.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#include "../headers/events.h"
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