#include <stdlib.h>
#include <string.h>
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
    SDL_Texture *texture;
    char *imagePath;

    bool keepAspectRatio;
};

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

float minf(float a, float b) {
    return (a < b) ? a : b;
}

float maxf(float a, float b) {
    return (a > b) ? a : b;
}

int main(int argc, char* argv[]) {
    // ----- INIT -----
    struct AppState state;
    state.window = NULL;
    state.renderer = NULL;
    state.texture = NULL;
    state.keepAspectRatio = true;

    state.imagePath = malloc(100 * sizeof(char));
    if (state.imagePath == NULL) {
        SDL_Log("Memory allocation failed\n");
        return 1;
    }
    strcpy(state.imagePath, IMG_PATH);

    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    state.window = SDL_CreateWindow("plainIMG", W_WIDTH, W_HEIGHT, SDL_WINDOW_RESIZABLE); // SDL_WINDOW_RESIZABLE or 0
    if (state.window == NULL) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }

    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (state.renderer == NULL) {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -3;
    }

    state.texture = IMG_LoadTexture(state.renderer, state.imagePath);
    if (state.texture == NULL) {
        //SDL_Log("IMG_LoadTexture error: %s", IMG_GetError());
        SDL_Log("IMG_LoadTexture error:");
        return -4;
    }

    SDL_Log("SDL3 initialized");

    // ----- MAIN -----
    // bug: the image only resizes when i stop dragging (unhold the mouse button)

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    SDL_Log("SDL3 event quit");
                    quit = 1;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0xff, 0xff);
        SDL_RenderClear(state.renderer);

        if (state.keepAspectRatio) {
            int _windowWidth, _windowHeight;
            SDL_GetWindowSize(state.window, &_windowWidth, &_windowHeight);

            //int _textureWidth, _textureHeight;
            //SDL_QueryTexture(state.texture, &_textureWidth, &_textureHeight);
            int _textureWidth = state.texture->w;
            int _textureHeight = state.texture->h;

            float _scale = minf((float)_windowWidth / (float)_textureWidth, (float)_windowHeight / (float)_textureHeight);
            //SDL_Log("window %d %d  texture %d %d   divided %f", _windowWidth, _windowHeight, _textureWidth, _textureHeight, (float)_windowWidth / (float)_textureWidth);

            // i should use SDL_FRect instead of SDL_Rect
            SDL_FRect destRect;
            destRect.w = (int)(_textureWidth * _scale);
            destRect.h = (int)(_textureHeight * _scale);
            destRect.x = (_windowWidth - destRect.w) / 2;
            destRect.y = (_windowHeight - destRect.h) / 2;
            //destRect.x = _windowWidth / 2;
            //destRect.y = _windowHeight / 2;

            SDL_RenderTexture(state.renderer, state.texture, NULL, &destRect);
        } else {
            SDL_RenderTexture(state.renderer, state.texture, NULL, NULL);
        }

        SDL_RenderPresent(state.renderer);
        SDL_Delay(1);
    }

    // ----- QUIT -----

    SDL_Log("SDL3 shutdown");
    SDL_DestroyTexture(state.texture);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    //IMG_Quit();
    SDL_Quit();
    free(state.imagePath);

    return 0;
}
