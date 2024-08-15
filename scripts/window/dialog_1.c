#include "../../headers/window/dialog_1.h"
#include "../../headers/window/startup.h"
#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Include SDL_ttf for text rendering
#include <stdio.h>
#include "../../headers/log.h"
#include <string.h>
void BeforeCallback(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        ShowStartupMenu(first_window,first_renderer);
        ClearEvents();
    }
}

void AfterCallback(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 2 clicked!\n");
    }
}

void CloseCallback(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
       *alive=0;
    }
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

void Dialog_1(SDL_Window* window, SDL_Renderer* renderer) {
    // Initialize log
    InitLogFile("logs.txt");
    // Main loop to handle events and render
    SDL_Event e;
    LogMessage("dialog 1 launch");
        // Clear the screen before rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set to black color
        SDL_RenderClear(renderer);

        // Render the background texture
        //SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        while(*alive){
        // Load and render the first image (448px height)
        SDL_Surface *image1Surface = IMG_Load("medias/images/dialog_1.png");
        if (!image1Surface) {
            LogMessage("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        SDL_Texture *image1Texture = SDL_CreateTextureFromSurface(renderer, image1Surface);
        SDL_FreeSurface(image1Surface);
        SDL_Rect image1Rect = {0,0, 650, 500};  // Example position (x: 100, y: 100)
        SDL_RenderCopy(renderer, image1Texture, NULL, &image1Rect);
        SDL_DestroyTexture(image1Texture);

        // Load and render the second image (112px height)
        SDL_Surface *image2Surface = IMG_Load("medias/images/bottom-board.png");
        if (!image2Surface) {
            LogMessage("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        SDL_Texture *image2Texture = SDL_CreateTextureFromSurface(renderer, image2Surface);
        SDL_FreeSurface(image2Surface);
        SDL_Rect image2Rect = {0, 490, 600, 122};  // Example position (x: 100, y: 600)
        SDL_RenderCopy(renderer, image2Texture, NULL, &image2Rect);
        const char* filename = "dialog_1"; // base filename
        const char* language = "en"; // language choice (e.g., "en" for English, "fr" for French)
        char* text = load_text(filename, language);
        // Render Lorem Ipsum text on the second image
        SDL_Color textColor = {255, 255, 255, 255};  // White text color
        RenderText(renderer,"Commander, battle is inevitable. Lead our fleet with precision and strength. The eyes of the world are upon us. Engage the enemy and secure victory.", image2Rect.x + 10, image2Rect.y + 10, textColor, 16, image2Rect.w - 20);

        SDL_DestroyTexture(image2Texture);

        // Render the buttons
        int width3 = 30;
        int height3 = 30;
        CreateClickableElement(renderer, 558, 11, &width3, &height3, NULL, textColor, "medias/images/btn-close.png", CloseCallback, 12);
        CreateClickableElement(renderer, 12, 11, &width3, &height3, NULL, textColor, "medias/images/left-arrow.png", BeforeCallback, 12);

        // Present the rendered content on the screen
        SDL_RenderPresent(renderer);

        // Handle events
       while (SDL_PollEvent(&e) != 0 && *alive) {
            if (e.type == SDL_QUIT) {
                *alive = 0; // Exit the loop if the window is closed
            }
            ProcessEvents(window, renderer);
        }
    }
    // Cleanup
   
   CloseLogFile();
}

