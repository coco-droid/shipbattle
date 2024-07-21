#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef enum {
    COLOR,
    IMAGE,
    TEXT
} UIElementType;

typedef struct UIElement {
    SDL_Rect rect;
    void (*onClick)(struct UIElement* element);
    void (*onHover)(struct UIElement* element);
    void (*onMouseIn)(struct UIElement* element);
    void (*onMouseOut)(struct UIElement* element);
    void *callback_data;
    struct UIElement* parent;
    struct UIElement** children;
    int children_count;
    UIElementType type;
    union {
        SDL_Color color;
        SDL_Texture* image;
        struct {
            TTF_Font* font;
            SDL_Color textColor;
            char* text;
            SDL_Texture* texture;
        } text;
    };
} UIElement;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    UIElement *root_element;
} UIWindow;

UIWindow* create_window(const char* title, int width, int height);
void destroy_window(UIWindow *ui_window);
UIElement* create_element(int x, int y, int width, int height, UIElementType type);
void add_child(UIElement *parent, UIElement *child);
void set_element_color(UIElement *element, SDL_Color color);
void set_element_image(UIElement *element, const char* image_path, SDL_Renderer *renderer);
void set_element_text(UIElement *element, const char* text, const char* font_path, int font_size, SDL_Color color, SDL_Renderer *renderer);
void set_on_click(UIElement *element, void (*callback)(UIElement*));
void set_on_hover(UIElement *element, void (*callback)(UIElement*));
void render_element(UIWindow *ui_window, UIElement *element);
void render_window(UIWindow *ui_window);
void handle_events(UIWindow *ui_window);

#endif // UI_H
