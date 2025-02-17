#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define WMIN_WIDTH 200
#define WMIN_HEIGHT 150
#define IMG_PATH "image.png"

// why did i have typedef here
struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    char imagePath[];
};

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    //if (IMG_Init(IMG_INIT_PNG) == 0) {
    //    SDL_Log("IMG_Init error: %s", IMG_GetError());
    //    SDL_Quit();
    //    return -1;
    //}

    window = SDL_CreateWindow("sdl3 hello world", W_WIDTH, W_HEIGHT, SDL_WINDOW_RESIZABLE); // SDL_WINDOW_RESIZABLE or 0
    if (window == NULL) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -3;
    }

    // Load image into texture
    texture = IMG_LoadTexture(renderer, IMG_PATH);
    if (texture == NULL) {
        //SDL_Log("IMG_LoadTexture error: %s", IMG_GetError());
        SDL_Log("IMG_LoadTexture error:");
        return -4;
    }

    SDL_Log("SDL3 initialized");

    // bug: the image only resizes when i stop dragging (unhold the mouse button)

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        bool resized = false;
        while (SDL_PollEvent(&event)) {
            SDL_Log("SDL_PollEvent");
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    SDL_Log("SDL3 event quit");
                    quit = 1;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    SDL_Log("SDL_PollEvent SDL_EVENT_WINDOW_RESIZED");
                    resized = true;
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION: {
                    SDL_Log("SDL_PollEvent SDL_EVENT_MOUSE_MOTION");
                    resized = true;
                    break;
                }
            }
            if (resized) {
                SDL_Log("SDL_PollEvent resized");
                break;
            }
        }

        SDL_Log("SDL3 render start");
        SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
        SDL_RenderClear(renderer);

        // Render image at (0, 0)
        SDL_RenderTexture(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
        SDL_Log("SDL3 render end");
    }

    SDL_Log("SDL3 shutdown");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //IMG_Quit();
    SDL_Quit();

    return 0;
}
