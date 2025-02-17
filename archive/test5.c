#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define IMG_PATH "image.png"

// https://gist.github.com/armornick/3434362

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture *img = NULL;
    int w, h;

    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("sdl3 hello world", WIDTH, HEIGHT, 0);
    if (window == NULL) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -3;
    }

    img = IMG_LoadTexture(renderer, IMG_PATH);
    //SDL_QueryTexture(img, NULL, NULL, &w, &h);
    SDL_FRect texr; texr.x = WIDTH/2; texr.y = HEIGHT/2; texr.w = w*2; texr.h = h*2; 

    SDL_Log("SDL3 initialized");

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_EVENT_QUIT: {
                    SDL_Log("SDL3 event quit");
                    quit = 1;
                    break;
                }
            }
        }

        //SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, img, NULL, &texr);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_Log("SDL3 shutdown");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
