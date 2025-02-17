#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define WMIN_WIDTH 200
#define WMIN_HEIGHT 150
#define IMG_PATH "terrain.png"

// TODO: add the image filename to the window's title
#define WINDOW_TITLE "plainIMG"
#define WINDOW_TILTE_TOP "plainIMG [TOP]"

#define CHECK_STATE(cond) ((cond) ? MF_CHECKED : MF_UNCHECKED)

// why did i have typedef here
struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    char *imagePath;

    bool keepAspectRatio;
    bool useBlackBg;
    bool alwaysOnTop;
    bool useAntialiasing;
};
//typedef struct _appstate AppState;

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

void updateAlwaysOnTop(struct AppState *state) {
    SDL_SetWindowAlwaysOnTop(state->window, state->alwaysOnTop);
    SDL_SetWindowTitle(state->window, state->alwaysOnTop ? WINDOW_TILTE_TOP : WINDOW_TITLE);
}

void updateUseAntialiasing(struct AppState *state) {
    SDL_SetTextureScaleMode(state->texture, state->useAntialiasing ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
}

#ifdef _WIN32
#include <windows.h>
void showContextMenu(struct AppState *state, int x, int y) {
    SDL_PropertiesID props = SDL_GetWindowProperties(state->window);
    if (props == 0) {
        SDL_Log("Failed to obtain SDL window properties.");
        return;
    }

    HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    if (hwnd == NULL) {
        SDL_Log("Failed to obtain hwnd from SDL window props.");
        return;
    }

    HMENU hMenu = CreatePopupMenu();
    if (hMenu == NULL) {
        SDL_Log("Failed to create popup menu.");
        return;
    }

    // it takes the first letter of the string and makes it the shortcut for the button
    // ugh why isnt there just an argument for this?
    AppendMenu(hMenu, MF_STRING, 1, "Open Image...\tO");
    AppendMenu(hMenu, MF_STRING, 2, "Close Image\tC");
    AppendMenu(hMenu, CHECK_STATE(state->alwaysOnTop), 3, "Window always on top\tT");
    AppendMenu(hMenu, CHECK_STATE(state->keepAspectRatio), 4, "Keep aspect ratio\tA");
    AppendMenu(hMenu, MF_STRING, 5, "Resize window to image\tR");
    AppendMenu(hMenu, CHECK_STATE(state->useBlackBg), 6, "Use black background\tB");
    AppendMenu(hMenu, CHECK_STATE(state->useAntialiasing), 7, "Use antialiasing\tL");
    AppendMenu(hMenu, MF_SEPARATOR, 10, "");
    AppendMenu(hMenu, MF_STRING, 8, "Quit\tQ");

    int itemId = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, x, y, 0, hwnd, NULL);
    SDL_Log("itemId: %d", itemId);
    switch (itemId) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            state->alwaysOnTop = !state->alwaysOnTop;
            updateAlwaysOnTop(state);
            break;
        case 4:
            state->keepAspectRatio = !state->keepAspectRatio;
            break;
        case 5:
            break;
        case 6:
            state->useBlackBg = !state->useBlackBg;
            break; // omg i forgot a break statement here and it almost broke the logic
        case 7:
            state->useAntialiasing = !state->useAntialiasing;
            updateUseAntialiasing(state);
            break;
        case 8:
            break;
    }

    DestroyMenu(hMenu);
}
#else
// context menu unsuported for other platforms, for now
void showContextMenu(struct AppState *state, int x, int y) {}
#endif

int main(int argc, char* argv[]) {
    // ----- INIT -----
    struct AppState state;
    state.window = NULL;
    state.renderer = NULL;
    state.texture = NULL;
    state.keepAspectRatio = true;
    state.useBlackBg = false; // TODO: default to system's dark mode with 'SystemParametersInfo(SPI_GETCLIENTAREAOFWINDOW, 0, &is_dark_mode, 0)' inside 'windows.h'
    state.alwaysOnTop = true;
    state.useAntialiasing = false;

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
        // the issue is that it was a webp file, even though it was png extension
        // im getting this weird error for low resolution image (terrain.png):
        // Failed loading libwebpdemux-2.dll: The specified module could not be found.
        // https://stackoverflow.com/questions/67082153/pygame-installed-on-windows-getting-pygame-error-failed-loading-libwebp-7-dll
        // https://stackoverflow.com/questions/74526664/sdl2-problems-with-webp-animated-images-works-with-gifs-but-not-with-webp
        // source code for IMG_LoadTexture: https://github.com/libsdl-org/SDL_image/blob/9ce9650a2bf8cf1c95d77ce8c5ce0a54f4ccbed4/src/IMG.c#L203
        SDL_Log("IMG_LoadTexture error: %s", SDL_GetError());
        //SDL_Log("IMG_LoadTexture error: %s", IMG_GetError());
        //SDL_Log("IMG_LoadTexture error:");
        return -4;
    }
    /*SDL_Surface* img = IMG_Load(IMG_PATH);
    if (!img) {
        SDL_Log("IMG_Load: %s", SDL_GetError());
    }
    state.texture = SDL_CreateTextureFromSurface(state.renderer, img);
    SDL_DestroySurface(img);*/

    updateAlwaysOnTop(&state);
    updateUseAntialiasing(&state);
    //SDL_Log("SDL3 initialized");

    // ----- MAIN -----
    // bug: the image only resizes when i stop dragging (unhold the mouse button)

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = 1;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        showContextMenu(&state, event.button.x, event.button.y);
                    }
                    break;

                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_Q:
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;

                        case SDLK_A:
                            state.keepAspectRatio = !state.keepAspectRatio;
                            break;

                        case SDLK_B:
                            state.useBlackBg = !state.useBlackBg;
                            break;

                        case SDLK_T:
                            state.alwaysOnTop = !state.alwaysOnTop;
                            updateAlwaysOnTop(&state);
                            break;

                        case SDLK_L:
                            state.useAntialiasing = !state.useAntialiasing;
                            updateUseAntialiasing(&state);
                            break;
                    }
                    break;
            }
        }

        int _bgColor = state.useBlackBg ? 0 : 0xff;
        SDL_SetRenderDrawColor(state.renderer, _bgColor, _bgColor, _bgColor, 0xff);
        SDL_RenderClear(state.renderer);

        if (state.keepAspectRatio) {
            int _windowWidth, _windowHeight;
            SDL_GetWindowSize(state.window, &_windowWidth, &_windowHeight);
            int _textureWidth = state.texture->w;
            int _textureHeight = state.texture->h;

            float _scale = minf((float)_windowWidth / (float)_textureWidth, (float)_windowHeight / (float)_textureHeight);

            SDL_FRect destRect;
            destRect.w = (int)(_textureWidth * _scale);
            destRect.h = (int)(_textureHeight * _scale);
            destRect.x = (_windowWidth - destRect.w) / 2;
            destRect.y = (_windowHeight - destRect.h) / 2;

            SDL_RenderTexture(state.renderer, state.texture, NULL, &destRect);
        } else {
            SDL_RenderTexture(state.renderer, state.texture, NULL, NULL);
        }

        SDL_RenderPresent(state.renderer);
        SDL_Delay(1);
    }

    // ----- QUIT -----

    //SDL_Log("SDL3 shutdown");
    SDL_DestroyTexture(state.texture);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    //IMG_Quit();
    SDL_Quit();
    free(state.imagePath);

    return 0;
}
