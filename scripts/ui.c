#include "../headers/ui.h"

UIWindow* create_window(const char* title, int width, int height) {
    UIWindow *ui_window = (UIWindow*)malloc(sizeof(UIWindow));
    ui_window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    ui_window->renderer = SDL_CreateRenderer(ui_window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ui_window->root_element = create_element(0, 0, width, height, COLOR);
    ui_window->root_element->color = (SDL_Color){255, 255, 255, 255}; // Default to white background
    return ui_window;
}

UIElement* create_element(int x, int y, int width, int height, UIElementType type) {
    UIElement *element = (UIElement*)malloc(sizeof(UIElement));
    element->rect = (SDL_Rect){x, y, width, height};
    element->onClick = NULL;
    element->onHover = NULL;
    element->onMouseIn = NULL;
    element->onMouseOut = NULL;
    element->callback_data = NULL;
    element->parent = NULL;
    element->children = NULL;
    element->children_count = 0;
    element->type = type;
    if (type == TEXT) {
        element->text.texture = NULL;
    }
    return element;
}

void add_child(UIElement *parent, UIElement *child) {
    parent->children = (UIElement**)realloc(parent->children, sizeof(UIElement*) * (parent->children_count + 1));
    parent->children[parent->children_count] = child;
    child->parent = parent;
    parent->children_count++;
}

void set_element_color(UIElement *element, SDL_Color color) {
    if (element->type == COLOR) {
        element->color = color;
    }
}

void set_element_image(UIElement *element, const char* image_path, SDL_Renderer *renderer) {
    if (element->type == IMAGE) {
        SDL_Surface *surface = IMG_Load(image_path);
        if (!surface) {
            printf("IMG_Load Error: %s\n", IMG_GetError());
            return;
        }
        element->image = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void set_element_text(UIElement *element, const char* text, const char* font_path, int font_size, SDL_Color color, SDL_Renderer *renderer) {
    if (element->type == TEXT) {
        element->text.font = TTF_OpenFont(font_path, font_size);
        if (!element->text.font) {
            printf("TTF_OpenFont Error: %s\n", TTF_GetError());
            return;
        }
        element->text.textColor = color;
        SDL_Surface *surface = TTF_RenderText_Solid(element->text.font, text, color);
        if (!surface) {
            printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
            return;
        }
        element->text.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        element->rect.w = surface->w;
        element->rect.h = surface->h;
    }
}

void set_on_click(UIElement *element, void (*callback)(UIElement*)) {
    element->onClick = callback;
}

void render_element(UIWindow *ui_window, UIElement *element) {
    switch (element->type) {
        case COLOR:
            SDL_SetRenderDrawColor(ui_window->renderer, element->color.r, element->color.g, element->color.b, element->color.a);
            SDL_RenderFillRect(ui_window->renderer, &element->rect);
            break;
        case IMAGE:
            SDL_RenderCopy(ui_window->renderer, element->image, NULL, &element->rect);
            break;
        case TEXT:
            SDL_RenderCopy(ui_window->renderer, element->text.texture, NULL, &element->rect);
            break;
    }

    for (int i = 0; i < element->children_count; ++i) {
        render_element(ui_window, element->children[i]);
    }
}

void render_window(UIWindow *ui_window) {
    SDL_SetRenderDrawColor(ui_window->renderer, 255, 255, 255, 255); // Clear with white color
    SDL_RenderClear(ui_window->renderer);
    render_element(ui_window, ui_window->root_element);
    SDL_RenderPresent(ui_window->renderer);
}

void handle_events(UIWindow *ui_window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Handle click events
        }
    }
}
void destroy_element(UIElement *element) {
    if (element->type == IMAGE && element->image) {
        SDL_DestroyTexture(element->image);
    } else if (element->type == TEXT && element->text.texture) {
        SDL_DestroyTexture(element->text.texture);
        TTF_CloseFont(element->text.font);
        free(element->text.text);
    }

    for (int i = 0; i < element->children_count; ++i) {
        destroy_element(element->children[i]);
    }

    free(element->children);
    free(element);
}

void destroy_window(UIWindow *ui_window) {
    destroy_element(ui_window->root_element);
    SDL_DestroyRenderer(ui_window->renderer);
    SDL_DestroyWindow(ui_window->window);
    free(ui_window);
}
