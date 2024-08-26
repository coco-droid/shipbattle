#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define MAX_CALLBACKS 50

typedef void (*EventCallback)(int);

extern int* alive;

typedef struct {
    int x, y, width, height;
    void (*onClick)(SDL_Event *event);
    void (*onHover)(SDL_Event *event);
    void (*onDragStart)(SDL_Event *event);
    void (*onDragMove)(SDL_Event *event);
    void (*onDragEnd)(SDL_Event *event);
} ClickableArea;

typedef struct {
    int x, y, width, height;
    SDL_Texture *backgroundTexture;  // Background texture for the input area
    char text[21];                   // The text entered by the user (up to 20 characters + null terminator)
    SDL_Color textColor;             // The color of the text
    TTF_Font *font;                  // The font used for rendering the text
    bool isDefaultText;              // Flag to indicate if the default text is displayed
    void (*onSubmit)(const char *);  // Callback function when the user submits the text
} TextInput;

void InitEvents(void);
void RegisterClickableArea(ClickableArea area);
void RegisterTextInputArea(TextInput *inputArea);
void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer);
void ClearEvents(void);
bool IsInArea(ClickableArea* area, int x, int y);
void RenderTextInputWithBackground(SDL_Renderer* renderer, SDL_Texture* backgroundTexture);

#endif // EVENTS_H
