#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define WMIN_WIDTH 150
#define WMIN_HEIGHT 150
#define REPO_URL "https://github.com/qaptivator/plainimg2"

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
    //char *imagePath;
    bool textureLoaded;
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
    if (state->textureLoaded) {
        SDL_SetTextureScaleMode(state->texture, state->useAntialiasing ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
    }
}

void resizeWindowToImage(struct AppState *state) {
    if (state->textureLoaded && state->keepAspectRatio && state->textureRect->w > 0 && state->textureRect->h > 0) {
        SDL_SetWindowSize(state->window, state->textureRect->w, state->textureRect->w);
    }
}

#ifdef _WIN32
#include <windows.h>

HWND getHwndFromWindow(SDL_Window *window) {
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    if (props == 0) {
        SDL_Log("Failed to obtain SDL window properties.");
        return NULL;
    }

    HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    if (hwnd == NULL) {
        SDL_Log("Failed to obtain hwnd from SDL window props.");
        return NULL;
    }

    return hwnd;
}

void showContextMenu(struct AppState *state, int x, int y) {
    HWND hwnd = getHwndFromWindow(state->window);

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
HWND getHwndFromWindow(SDL_Window *window) {}
#endif

void drawFrame(struct AppState *state) {
    int _bgColor = state->useBlackBg ? 0 : 0xff;
    SDL_SetRenderDrawColor(state->renderer, _bgColor, _bgColor, _bgColor, 0xff);
    SDL_RenderClear(state->renderer);

    if (state->textureLoaded) {
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
    } else {}

    SDL_RenderPresent(state->renderer);
    SDL_Delay(1);
}

void handleEvent(struct AppState *state, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            state->quitApp = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            // GetCursorPos IS RELATIVE TO SCREEN, event->button.x OR event->motion.x IS RELATIVE TO WINDOW
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
                POINT pt;
                GetCursorPos(&pt);
                int deltaX = pt.x - state->dragX;
                int deltaY = pt.y - state->dragY;
                SDL_SetWindowPosition(state->window, deltaX, deltaY);
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

void openImage(struct AppState *state, const char *path) {
    HWND hwnd = getHwndFromWindow(state->window);

    // close SDL_IOStream with SDL_CloseIO(stream);
    SDL_IOStream *stream = SDL_IOFromFile(path, "r");
    if (stream == NULL) {
        // file doesnt exist
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }

    if (IMG_isWEBP(stream)) {
        SDL_Log("WebP is not supported in plainIMG. Eiether convert it to a different image format, or select another image.");
        MessageBox(hwnd, TEXT("WebP is not supported in plainIMG. Eiether convert it to a different image format, or select another image."), TEXT("plainIMG Error"), MB_OK | MB_ICONERROR);
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }

    // the last option automatically closes the stream
    SDL_Texture *newTexture = IMG_LoadTexture_IO(state->renderer, stream, true);
    if (newTexture == NULL) {
        // SDL3_image CANNOT parse webp files. or else you get this error:
        // Failed loading libwebpdemux-2.dll: The specified module could not be found.
        SDL_Log("IMG_LoadTexture error at setImagePath: %s", SDL_GetError());
        return;
    }

    state->texture = newTexture;
    state->textureLoaded = true;
}

int main(int argc, char* argv[]) {
    // ----- INIT -----
    struct AppState state;
    state.window = NULL;
    state.renderer = NULL;
    state.texture = NULL;
    state.textureLoaded = false;
    state.keepAspectRatio = true;
    state.useBlackBg = false; // TODO: default to system's dark mode with 'SystemParametersInfo(SPI_GETCLIENTAREAOFWINDOW, 0, &is_dark_mode, 0)' inside 'windows.h'
    state.alwaysOnTop = true;
    state.useAntialiasing = true;
    state.quitApp = false;
    state.dragging = false;
    state.dragX = 0;
    state.dragY = 0;

    SDL_FRect _textureRect;
    state.textureRect = &_textureRect;

    // init
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    // window
    state.window = SDL_CreateWindow("plainIMG", W_WIDTH, W_HEIGHT, SDL_WINDOW_RESIZABLE); // SDL_WINDOW_RESIZABLE or 0
    if (state.window == NULL) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }
    SDL_SetWindowMinimumSize(state.window, WMIN_WIDTH, WMIN_HEIGHT);

    // renderer
    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (state.renderer == NULL) {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -3;
    }

    // other
    SDL_SetEventFilter(&eventFilter, &state);
    updateAlwaysOnTop(&state);
    updateUseAntialiasing(&state);
    // SDL_Log("SDL3 initialized");

    // ----- MAIN -----

    if (argc > 1) {
        openImage(&state, argv[1]);
    }

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
    //free(state.imagePath);

    return 0;
}
