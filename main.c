#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define WMIN_WIDTH 150
#define WMIN_HEIGHT 150
#define REPO_URL "https://github.com/qaptivator/plainimg"
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
    SDL_FRect *textureRect;
    char *imagePath;
    bool quitApp;

    bool keepAspectRatio;
    bool useBlackBg;
    bool alwaysOnTop;
    bool useAntialiasing;

    bool dragging;
    int dragX, dragY;
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

void resizeWindowToImage(struct AppState *state) {
    if (state->keepAspectRatio && state->textureRect->w > 0 && state->textureRect->h > 0) {
        SDL_SetWindowSize(state->window, state->textureRect->w, state->textureRect->w);
    }
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
    AppendMenu(hMenu, MF_STRING, 8, "About");
    AppendMenu(hMenu, MF_STRING, 9, "Quit\tQ");

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
            resizeWindowToImage(state);
            break;
        case 6:
            state->useBlackBg = !state->useBlackBg;
            break;
        case 7:
            state->useAntialiasing = !state->useAntialiasing;
            updateUseAntialiasing(state);
            break;
        case 8:
            int _buttonId = MessageBox(hwnd, TEXT("plainIMG. as simple as it gets for an image viewer. made by qaptivator, licensed under MIT. to see the GitHub repository, click 'OK'"), TEXT( "About plainIMG" ), MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2);
            SDL_Log("_buttonId: %d", _buttonId);
            if (_buttonId == 1) {
                //system("open https://github.com/qaptivator/plainimg");
                ShellExecute(NULL, "open", REPO_URL, NULL, NULL, SW_SHOWNORMAL);
            }
            break;
        case 9:
            state->quitApp = true;
            break;
    }

    DestroyMenu(hMenu);
}
#else
// context menu unsuported for other platforms, for now
void showContextMenu(struct AppState *state, int x, int y) {}
#endif

void drawFrame(struct AppState *state) {
    int _bgColor = state->useBlackBg ? 0 : 0xff;
    SDL_SetRenderDrawColor(state->renderer, _bgColor, _bgColor, _bgColor, 0xff);
    SDL_RenderClear(state->renderer);

    if (state->keepAspectRatio) {
        int _windowWidth, _windowHeight;
        SDL_GetWindowSize(state->window, &_windowWidth, &_windowHeight);
        int _textureWidth = state->texture->w;
        int _textureHeight = state->texture->h;

        float _scale = minf((float)_windowWidth / (float)_textureWidth, (float)_windowHeight / (float)_textureHeight);

        state->textureRect->w = (int)(_textureWidth * _scale);
        state->textureRect->h = (int)(_textureHeight * _scale);
        state->textureRect->x = (_windowWidth - state->textureRect->w) / 2;
        state->textureRect->y = (_windowHeight - state->textureRect->h) / 2;

        SDL_RenderTexture(state->renderer, state->texture, NULL, state->textureRect);
    } else {
        SDL_RenderTexture(state->renderer, state->texture, NULL, NULL);
    }

    SDL_RenderPresent(state->renderer);
    SDL_Delay(1);
}

void handleEvent(struct AppState *state, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            state->quitApp = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            POINT pt;
            GetCursorPos(&pt);
            if (event->button.button == SDL_BUTTON_RIGHT) {
                showContextMenu(state, pt.x, pt.y);
                //showContextMenu(&state, event->button.x, event->button.y);
            }
            if (event->button.button == SDL_BUTTON_LEFT) {
                state->dragging = true;
                int windowX, windowY;
                SDL_GetWindowPosition(state->window, &windowX, &windowY);
                //state->dragX = event->button.x - windowX;
                //state->dragY = event->button.y - windowY;
                state->dragX = pt.x - windowX;
                state->dragY = pt.y - windowY;
            }
            break;
        
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event->button.button == SDL_BUTTON_LEFT) {
                state->dragging = false;
            }
            break;

        // https://stackoverflow.com/questions/7773771/how-do-i-implement-dragging-a-window-using-its-client-area
        case SDL_EVENT_MOUSE_MOTION:
            if (state->dragging) {
                //int deltaX = event->motion.x - state->dragX;
                //int deltaY = event->motion.y - state->dragY;

                POINT pt;
                GetCursorPos(&pt);
                int deltaX = pt.x - state->dragX;
                int deltaY = pt.y - state->dragY;
                SDL_SetWindowPosition(state->window, deltaX, deltaY);

                //int windowX, windowY;
                //SDL_GetWindowPosition(state->window, &windowX, &windowY);

                // event->motion.x - deltaX, event->motion.y - deltaY
                // windowX + deltaX, windowY + deltaY
                //SDL_SetWindowPosition(state->window, windowX + deltaX, windowY + deltaY);

                //state->dragX = event->motion.x;
                //state->dragY = event->motion.y;
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_Q:
                case SDLK_ESCAPE:
                    state->quitApp = true;
                    break;

                case SDLK_A:
                    state->keepAspectRatio = !state->keepAspectRatio;
                    break;

                case SDLK_B:
                    state->useBlackBg = !state->useBlackBg;
                    break;

                case SDLK_T:
                    state->alwaysOnTop = !state->alwaysOnTop;
                    updateAlwaysOnTop(state);
                    break;

                case SDLK_L:
                    state->useAntialiasing = !state->useAntialiasing;
                    updateUseAntialiasing(state);
                    break;

                case SDLK_R:
                    resizeWindowToImage(state);
                    break;
            }
            break;
    }
}

// https://stackoverflow.com/questions/7106586/keep-window-active-while-being-dragged-sdl-on-win32
// https://wiki.libsdl.org/SDL3/SDL_EventFilter
// https://wiki.libsdl.org/SDL3/SDL_SetEventFilter
bool eventFilter(void *userdata, SDL_Event *event) {
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        struct AppState *state = (struct AppState*)userdata;
        // Note: NULL rectangle is the entire window
        SDL_SetRenderViewport(state->renderer, NULL);
        drawFrame(state);
    }
    return 1;
}

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
    state.quitApp = false;
    state.dragging = false;
    state.dragX = 0;
    state.dragY = 0;

    SDL_FRect _textureRect;
    state.textureRect = &_textureRect;

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
    SDL_SetWindowMinimumSize(state.window, WMIN_WIDTH, WMIN_HEIGHT);

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
        return -4;
    }

    SDL_SetEventFilter(&eventFilter, &state);
    updateAlwaysOnTop(&state);
    updateUseAntialiasing(&state);
    //SDL_Log("SDL3 initialized");

    // ----- MAIN -----

    SDL_Event event;
    while (!state.quitApp) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&state, &event);
        }
        drawFrame(&state);
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
