#include "../headers/events.h"
#include "../headers/log.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

static ClickableArea clickableAreas[MAX_CALLBACKS];
static TextInput *textInputArea = NULL;
static int numAreas = 0;
static bool isDragging = false;
static ClickableArea* currentDragArea = NULL;
int aliveValue = 1;
int *alive = &aliveValue;

void InitEvents(void) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        clickableAreas[i].onClick = NULL;
        clickableAreas[i].onHover = NULL;
        clickableAreas[i].onDragStart = NULL;
        clickableAreas[i].onDragMove = NULL;
        clickableAreas[i].onDragEnd = NULL;
    }
    textInputArea = NULL;
    numAreas = 0;
}

void RegisterClickableArea(ClickableArea area) {
    if (numAreas < MAX_CALLBACKS) {
        clickableAreas[numAreas] = area;
        numAreas++;
    } else {
        fprintf(stderr, "No space left to register a new clickable area\n");
    }
}

void RegisterTextInputArea(TextInput *inputArea) {
    textInputArea = inputArea;
}

bool IsInArea(ClickableArea* area, int x, int y) {
    return (x >= area->x && x <= area->x + area->width &&
            y >= area->y && y <= area->y + area->height);
}

void HandleTextInput(SDL_Event *event) {
    if (textInputArea != NULL) {
        if (event->type == SDL_TEXTINPUT) {
            if (textInputArea->isDefaultText) {
                strcpy(textInputArea->text, "");
                textInputArea->isDefaultText = false;
            }
            if (strlen(textInputArea->text) < 20) {
                strcat(textInputArea->text, event->text.text);
            }
        } else if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(textInputArea->text) > 0) {
                textInputArea->text[strlen(textInputArea->text) - 1] = '\0';
                if (strlen(textInputArea->text) == 0) {
                    strcpy(textInputArea->text, "Your Name");
                    textInputArea->isDefaultText = true;
                }
            } else if (event->key.keysym.sym == SDLK_RETURN) {
                if (textInputArea->onSubmit) {
                    textInputArea->onSubmit(textInputArea->text);
                }
            }
        }
    }
}

void RenderTextInputWithBackground(SDL_Renderer* renderer, SDL_Texture* backgroundTexture) {
    if (textInputArea != NULL) {
        // Render the background image
        SDL_Rect destRect = { textInputArea->x, textInputArea->y, textInputArea->width, textInputArea->height };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &destRect);

        // Render the text on top of the background
        SDL_Surface* textSurface = TTF_RenderText_Blended(textInputArea->font, textInputArea->text, textInputArea->textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        SDL_Rect textRect = { textInputArea->x + 10, textInputArea->y + 10, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
}

void TriggerClickCallbacks(int x, int y) {
    for (int i = 0; i < numAreas; i++) {
        ClickableArea* area = &clickableAreas[i];

        if (IsInArea(area, x, y)) {
            if (area->onClick) {
                // Créer un événement SDL pour passer au callback
                SDL_Event clickEvent;
                clickEvent.type = SDL_MOUSEBUTTONDOWN;
                clickEvent.button.x = x;
                clickEvent.button.y = y;

                // Appeler le callback de clic
                area->onClick(&clickEvent);
            }
        }
    }
}

void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer) {
    InitLogFile("logs.txt");
    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                if (alive) {
                    *alive = 0;
                }
                quit = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION) {
                for (int i = 0; i < numAreas; i++) {
                    ClickableArea* area = &clickableAreas[i];

                    if (e.type == SDL_MOUSEBUTTONDOWN && IsInArea(area, e.button.x, e.button.y)) {
                        if (area->onClick) {
                            area->onClick(&e);
                        }
                        if (area->onDragStart) {
                            isDragging = true;
                            currentDragArea = area;
                            area->onDragStart(&e);
                        }
                    }

                    if (e.type == SDL_MOUSEBUTTONUP && isDragging) {
                        if (currentDragArea && currentDragArea->onDragEnd) {
                            currentDragArea->onDragEnd(&e);
                        }
                        isDragging = false;
                        currentDragArea = NULL;
                    }

                    if (e.type == SDL_MOUSEMOTION) {
                        if (IsInArea(area, e.motion.x, e.motion.y)) {
                            if (area->onHover) {
                                area->onHover(&e);
                            }
                            if (isDragging && currentDragArea && currentDragArea->onDragMove) {
                                currentDragArea->onDragMove(&e);
                            }
                        }
                    }
                }
            }

            HandleTextInput(&e);
        }

        // Render the text input area with the background image
        if (textInputArea != NULL) {
            RenderTextInputWithBackground(renderer, textInputArea->backgroundTexture);
        }

        SDL_RenderPresent(renderer);
    }
}

void ClearEvents(void) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        clickableAreas[i].onClick = NULL;
        clickableAreas[i].onHover = NULL;
        clickableAreas[i].onDragStart = NULL;
        clickableAreas[i].onDragMove = NULL;
        clickableAreas[i].onDragEnd = NULL;
    }
    textInputArea = NULL;
    numAreas = 0;
}

