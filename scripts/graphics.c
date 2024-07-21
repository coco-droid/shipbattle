#include "../headers/graphics.h"
#include <SDL2/SDL_syswm.h>
#include <windows.h>

// Function to set rounded corners for an existing window char* window_title=load_text('window_title','en');
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

// Function to create a window with rounded corners
SDL_Window* CreateRoundedWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window) {
        SetRoundedCorners(window);
    }
    return window;
}
