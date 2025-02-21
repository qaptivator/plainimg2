#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include "plainIMG_rc.h"
#include "plainSTR.h"

#ifndef PLAINIMG_VERSION
#define PLAINIMG_VERSION "UNKNOWN"
#endif

#define W_WIDTH 800
#define W_HEIGHT 600
#define WMIN_WIDTH 150
#define WMIN_HEIGHT 150
#define MAX_PATH 1000
#define REPO_URL "https://github.com/qaptivator/plainimg2"

#define SETTINGS_FILE_NAME "settings.txt"
#define SETTINGS_FILE_MESSAGE "zero is false, one is true\norder matters here\nkeepAspectRatio useBlackBg useAntialiasing alwaysOnTop keepWindowAspectRatio showImageName\n"
#define OPEN_IMAGE_FILTER "Image Files\0*.BMP;*.JPG;*.JPEG;*.PNG;*.GIF\0All Files\0*.*\0";

#define WINDOW_TITLE "plainIMG"
#define WINDOW_TITLE_TOP " [TOP]"
#define WINDOW_TITLE_WK " [WK]"

#define CONCAT(a, b) a b
#define CHECK_STATE(cond) ((cond) ? MF_CHECKED : MF_UNCHECKED)
#define BOOL_TO_INT(cond) ((cond) ? '1' : '0')
#define RETURN_IF_NULL(var) \
    do {                    \
        if ((var) == NULL) {\
            return NULL;    \
        }                   \
    } while(0)


// why did i have typedef here
struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_FRect *textureRect;
    //char *imagePath;
    PS_String *imageName;
    bool textureLoaded;
    bool quitApp;
    bool needRedraw;

    bool keepAspectRatio;
    bool useBlackBg;
    bool useAntialiasing;
    bool alwaysOnTop;
    bool keepWindowAspectRatio;
    bool showImageName;

    bool dragging;
    int dragX, dragY;

    SDL_Texture *welcomeWhite;
    SDL_Texture *welcomeBlack;
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

/*void stripRange(char *str, int start, int end) {
    int len = strlen(str);
    if (start < 0 || end > len || start >= end) return;
    memmove(&str[start], &str[end], len - end + 1);
}

void insertStr(char *str, int index, const char *insert, int buf_size) {
    int len = strlen(str);
    int ins_len = strlen(insert);
    if (index < 0 || index > len || len + ins_len >= buf_size) return;
    memmove(&str[index + ins_len], &str[index], len - index + 1);
    memcpy(&str[index], insert, ins_len);
}*/

void readStateFromFile(struct AppState *state) {
    FILE *file = fopen(SETTINGS_FILE_NAME, "r");
    if (!file) {
        SDL_Log("Couldn't open settings.txt for reading! Going back to default settings.");
        return;
    }

    int ch;
    int index = 0;
    bool value = false;

    // ORDER: keepAspectRatio useBlackBg useAntialiasing alwaysOnTop keepWindowAspectRatio showImageName
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '0' || ch == '1') {
            value = ch == '1';
            switch (index) {
                // I COMPLETELY FORGOT BREAK STATEMENTS LOL
                case 0:
                    state->keepAspectRatio       = value;
                case 1:
                    state->useBlackBg            = value;
                case 2:
                    state->useAntialiasing       = value;
                case 3:
                    state->alwaysOnTop           = value;
                case 4:
                    state->keepWindowAspectRatio = value;
                case 5:
                    SDL_Log("showImageName readStateFromFile bool: %i", value);
                    state->showImageName         = value;
            }
            index++;
        }
    }

    fclose(file);
}

void writeStateToFile(struct AppState *state) {
    FILE *file = fopen(SETTINGS_FILE_NAME, "w");
    if (!file) {
        SDL_Log("Couldn't open settings.txt for writing! Failed saving your settings.");
        return;
    }

    fprintf(file, SETTINGS_FILE_MESSAGE);
    fputc(BOOL_TO_INT(state->keepAspectRatio       ), file);
    fputc(BOOL_TO_INT(state->useBlackBg            ), file);
    fputc(BOOL_TO_INT(state->useAntialiasing       ), file);
    fputc(BOOL_TO_INT(state->alwaysOnTop           ), file);
    fputc(BOOL_TO_INT(state->keepWindowAspectRatio ), file);
    fputc(BOOL_TO_INT(state->showImageName         ), file);
    SDL_Log("showImageName writeStateToFile bool: %i", state->showImageName);
    SDL_Log("showImageName writeStateToFile: %c", BOOL_TO_INT(state->showImageName));

    fclose(file);
}

/*char* getStaticWindowTitle(struct AppState *state) {
    return state->alwaysOnTop ? CONCAT(WINDOW_TITLE, WINDOW_TITLE_TOP) : WINDOW_TITLE;
}*/

/*void updateWindowTitle(struct AppState *state, const char *imagePath) {
    /*const char *staticTitle = getStaticWindowTitle(state);
    if (state->textureLoaded) {
        if (imagePath != NULL) {
            char newTitle[strlen(imagePath) + strlen(staticTitle)];
            strcpy(newTitle, imagePath);
            strcat(newTitle, staticTitle);
            SDL_SetWindowTitle(state->window, newTitle);
            return;
        } else {
            char *title = SDL_GetWindowTitle(state->window);
            if (state->alwaysOnTop) {

            } else {

            }
        }
    } else {
        SDL_SetWindowTitle(state->window, staticTitle);
    }
}*/

/*void updateWindowTitle(struct AppState *state) {
    // i kind of hate this logic lol
    SDL_SetWindowTitle(state->window, 
        state->keepWindowAspectRatio
            ? state->alwaysOnTop 
                ? CONCAT(WINDOW_TITLE, CONCAT(WINDOW_TITLE_TOP, WINDOW_TITLE_WK))
                : CONCAT(WINDOW_TITLE, WINDOW_TITLE_WK)
            : state->alwaysOnTop 
                ? CONCAT(WINDOW_TITLE, WINDOW_TITLE_TOP)
                : WINDOW_TITLE
    );
}*/

void updateWindowTitle(struct AppState *state) {
    char *staticText = state->keepWindowAspectRatio
            ? state->alwaysOnTop 
                ? CONCAT(WINDOW_TITLE, CONCAT(WINDOW_TITLE_TOP, WINDOW_TITLE_WK))
                : CONCAT(WINDOW_TITLE, WINDOW_TITLE_WK)
            : state->alwaysOnTop 
                ? CONCAT(WINDOW_TITLE, WINDOW_TITLE_TOP)
                : WINDOW_TITLE;

    char fullText[strlen(staticText) + state->imageName->size + 4];
    fullText[0] = '\0';

    strcat(fullText, staticText);
    if (state->textureLoaded && state->showImageName) {
        strcat(fullText, " \"");
        strcat(fullText, PS_Get(state->imageName));
        strcat(fullText, "\"");
    }
    SDL_SetWindowTitle(state->window, fullText);
}

void calculateTextureRect(struct AppState *state) {
    int _windowWidth, _windowHeight;
    SDL_GetWindowSize(state->window, &_windowWidth, &_windowHeight);

    if (state->textureLoaded) {
        int _textureWidth = state->texture->w;
        int _textureHeight = state->texture->h;

        float _scale = minf((float)_windowWidth / (float)_textureWidth, (float)_windowHeight / (float)_textureHeight);

        state->textureRect->w = (int)(_textureWidth * _scale);
        state->textureRect->h = (int)(_textureHeight * _scale);
        state->textureRect->x = (_windowWidth - state->textureRect->w) / 2;
        state->textureRect->y = (_windowHeight - state->textureRect->h) / 2;
        
        if (state->keepAspectRatio) {
            SDL_RenderTexture(state->renderer, state->texture, NULL, state->textureRect);
        } else {
            SDL_RenderTexture(state->renderer, state->texture, NULL, NULL);
        }
    } else {
        if (state->useBlackBg) {
            state->textureRect->w = state->welcomeBlack->w;
            state->textureRect->h = state->welcomeBlack->h;
            state->textureRect->x = (_windowWidth - state->textureRect->w) / 2;
            state->textureRect->y = (_windowHeight - state->textureRect->h) / 2;
            SDL_RenderTexture(state->renderer, state->welcomeBlack, NULL, state->textureRect);
        } else {
            state->textureRect->w = state->welcomeWhite->w;
            state->textureRect->h = state->welcomeWhite->h;
            state->textureRect->x = (_windowWidth - state->textureRect->w) / 2;
            state->textureRect->y = (_windowHeight - state->textureRect->h) / 2;
            SDL_RenderTexture(state->renderer, state->welcomeWhite, NULL, state->textureRect);
        }
    }
}

void updateAlwaysOnTop(struct AppState *state) {
    SDL_SetWindowAlwaysOnTop(state->window, state->alwaysOnTop);
    //updateWindowTitle(state);
    //SDL_SetWindowTitle(state->window, state->alwaysOnTop ? WINDOW_TITLE_TOP : WINDOW_TITLE);
    updateWindowTitle(state);
}

void updateUseAntialiasing(struct AppState *state) {
    if (state->textureLoaded) {
        SDL_SetTextureScaleMode(state->texture, state->useAntialiasing ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
    }
}

void updateKeepWindowAspectRatio(struct AppState *state) {
    //state->keepWindowAspectRatio = !state->keepWindowAspectRatio;
    updateWindowTitle(state);
    if (state->keepWindowAspectRatio) {
        //state->keepAspectRatio = true;
        //resizeWindowToImage(state);
        int _windowWidth, _windowHeight;
        SDL_GetWindowSize(state->window, &_windowWidth, &_windowHeight);
        float _ratio = (float)_windowWidth / (float)_windowHeight;
        //SDL_Log("_ratio: %f %d %d", _ratio, _windowWidth, _windowHeight);
        SDL_SetWindowAspectRatio(state->window, _ratio, _ratio);
    } else {
        SDL_SetWindowAspectRatio(state->window, 0, 0);
    }
}

void resizeWindowToImage(struct AppState *state) {
    calculateTextureRect(state); // recalculate textureRect
    if (state->textureLoaded && state->keepAspectRatio && state->textureRect->w > 0 && state->textureRect->h > 0) {
        if (state->keepWindowAspectRatio) {
            SDL_SetWindowAspectRatio(state->window, 0, 0);
        }
        SDL_SetWindowSize(state->window, state->textureRect->w, state->textureRect->h);
        updateKeepWindowAspectRatio(state);
        //int _windowWidth, _windowHeight;
        //SDL_GetWindowSize(state->window, &_windowWidth, &_windowHeight);
        //SDL_SetWindowAspectRatio(state->window, _windowWidth / _windowHeight, _windowWidth / _windowHeight);
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

// unfortunately this function is windows-only now womp womp
void openImage(struct AppState *state, char *path) {
    state->needRedraw = true;

    if (path == NULL) {
        state->textureLoaded = false;
        updateKeepWindowAspectRatio(state);
        return;
    }

    HWND hwnd = getHwndFromWindow(state->window);

    SDL_IOStream *stream = SDL_IOFromFile(path, "r");
    if (stream == NULL) {
        // file doesnt exist
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }

    /*if (IMG_isWEBP(stream)) {
        SDL_Log("WebP is not supported in plainIMG. Eiether convert it to a different image format, or select another image.");
        MessageBox(hwnd, TEXT("WebP is not supported in plainIMG. Eiether convert it to a different image format, or select another image."), TEXT("plainIMG Error"), MB_OK | MB_ICONERROR);
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }
    if (IMG_isTIF(stream)) {
        SDL_Log("TIFF is not supported in plainIMG. Eiether convert it to a different image format, or select another image.");
        MessageBox(hwnd, TEXT("TIFF is not supported in plainIMG. Eiether convert it to a different image format, or select another image."), TEXT("plainIMG Error"), MB_OK | MB_ICONERROR);
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }
    if (IMG_isAVIF(stream)) {
        SDL_Log("AVIF is not supported in plainIMG. Eiether convert it to a different image format, or select another image.");
        MessageBox(hwnd, TEXT("AVIF is not supported in plainIMG. Eiether convert it to a different image format, or select another image."), TEXT("plainIMG Error"), MB_OK | MB_ICONERROR);
        state->textureLoaded = false;
        SDL_CloseIO(stream);
        return;
    }*/

    // the last option automatically closes the stream
    SDL_Texture *newTexture = IMG_LoadTexture_IO(state->renderer, stream, true);
    if (newTexture == NULL) {
        // SDL3_image CANNOT parse webp files. or else you get this error:
        // Failed loading libwebpdemux-2.dll: The specified module could not be found.
        MessageBox(hwnd, TEXT("Couldn't open the image! Your file format is either not supported, or these was an internal error. Currently unsupported file formats: WebP, TIFF, AVIF."), TEXT("plainIMG Error"), MB_OK | MB_ICONERROR);
        SDL_Log("IMG_LoadTexture error at setImagePath: %s", SDL_GetError());
        return;
    }

    state->texture = newTexture;
    state->textureLoaded = true;
    PS_Set(state->imageName, basename(path));

    updateUseAntialiasing(state);
    resizeWindowToImage(state);
    updateKeepWindowAspectRatio(state);
}

void openImageWithDialog(struct AppState *state) {
    // i think you should use SDL_filedialog instead
    // and SDL_ttf for text instead of an static image
    HWND hwnd = getHwndFromWindow(state->window);

    char filePath[MAX_PATH] = {0};

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = sizeof(filePath);
    //ofn.lpstrFilter = "Image Files\0*.BMP;*.JPG;*.JPEG;*.PNG;*.GIF;*.TIFF;*.WEBP\0All Files\0*.*\0";
    ofn.lpstrFilter = OPEN_IMAGE_FILTER;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL; 
    ofn.lpstrTitle = "Open Image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        openImage(state, ofn.lpstrFile);
    } else {
        MessageBox(hwnd, TEXT("No image was selected!"), TEXT("plainIMG Warning"), MB_OK | MB_ICONWARNING);
    }
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
    AppendMenu(hMenu, CHECK_STATE(state->keepWindowAspectRatio), 11, "Keep window aspect ratio\tW");
    AppendMenu(hMenu, MF_STRING, 5, "Resize window to image\tR");
    AppendMenu(hMenu, CHECK_STATE(state->useBlackBg), 6, "Use black background\tB");
    AppendMenu(hMenu, CHECK_STATE(state->useAntialiasing), 7, "Use antialiasing\tL");
    AppendMenu(hMenu, CHECK_STATE(state->showImageName), 12, "Show image name\tS");
    AppendMenu(hMenu, MF_SEPARATOR, 10, "");
    AppendMenu(hMenu, MF_STRING, 8, "About");
    AppendMenu(hMenu, MF_STRING, 9, "Quit\tQ");

    int itemId = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, x, y, 0, hwnd, NULL);
    switch (itemId) {
        case 1:
            openImageWithDialog(state);
            break;
        case 2:
            openImage(state, NULL);
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
            int _buttonId = MessageBox(
                hwnd,
                TEXT("plainIMG v") TEXT(PLAINIMG_VERSION) TEXT(" // as simple as it gets for an image viewer. made by qaptivator, licensed under MIT. to see the GitHub repository, click 'OK'"),
                TEXT("About plainIMG"), 
                MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2
            );
            SDL_Log("_buttonId: %d", _buttonId);
            if (_buttonId == 1) {
                //system("open https://github.com/qaptivator/plainimg");
                ShellExecute(NULL, "open", REPO_URL, NULL, NULL, SW_SHOWNORMAL);
            }
            break;
        case 9:
            state->quitApp = true;
            break;
        case 11:
            state->keepWindowAspectRatio = !state->keepWindowAspectRatio;
            updateKeepWindowAspectRatio(state);
            break;
        case 12:
            state->showImageName = !state->showImageName;
            updateWindowTitle(state);
            break;
    }

    state->needRedraw = true;
    DestroyMenu(hMenu);
}

/*HBITMAP loadBitmapFromResource(HINSTANCE hInstance, int resourceId) {
    return (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(resourceId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
}

SDL_Surface* convertBitmapToSDLSurface(HBITMAP hBitmap) {
    BITMAP bm;
    GetObject(hBitmap, sizeof(BITMAP), &bm);

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
        bm.bmBits,
        bm.bmWidth,
        bm.bmHeight,
        bm.bmBitsPixel,
        bm.bmWidthBytes,
        SDL_PIXELFORMAT_BGR24
    );

    return surface;
}

SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}*/

SDL_Texture* loadRcBitmapAsTexture(SDL_Renderer *renderer, int resourceId) {
    // OH MY GOD. THE BMP WASNT THE CORRECT FORMAT.
    // IT NEEDED TO BE 24 BIT COLOR INSTEAD OF THE "Windows 98/2000 and newer format"
    // I HATE CONVERTIO!!!!!!!!!!! IT CONVERTED PNG TO BMP WRONGLY
    // (oh wait, it included alpha channel for me. looks like i shouldve just knew this beforehand...)

    //SDL_Log("hBitmap loadRcBitmapAsTexture");
    HBITMAP hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceId), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    // here it returns error 1814, ERROR_RESOURCE_NAME_NOT_FOUND. solution: make sure that numeric ids match both in c files and in resource rc files.
    // now it has error 5, which means there is no access to something. solution: make sure that your bitmap is 24bit format (without alpha channel). you can check this with "file" command.
    if (hBitmap == NULL) {
        SDL_Log("hBitmap loadRcBitmapAsTexture error %d", GetLastError());
        DeleteObject(hBitmap);
        return NULL;
    }
    //RETURN_IF_NULL(hBitmap);

    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmap
    //SDL_Log("bitmap loadRcBitmapAsTexture");
    BITMAP bitmap;
    if (GetObject(hBitmap, sizeof(BITMAP), &bitmap) == 0) {
        SDL_Log("bitmap loadRcBitmapAsTexture error %d", GetLastError());
        DeleteObject(hBitmap);
        return NULL;
    }
    //RETURN_IF_NULL(&bitmap);
    // i think i should ensure that the bitmap is 24bit... but im lazy

    //SDL_Log("surface loadRcBitmapAsTexture: w%d h%d wb%d id%d", bitmap.bmWidth, bitmap.bmHeight, bitmap.bmWidthBytes, resourceId);
    SDL_Surface* surface = SDL_CreateSurfaceFrom(
        (int)bitmap.bmWidth,
        (int)bitmap.bmHeight,
        SDL_PIXELFORMAT_BGR24,
        (void*)bitmap.bmBits,
        (int)bitmap.bmWidthBytes
    );
    //SDL_Log("surface loadRcBitmapAsTexture w %d", surface->format);
    //RETURN_IF_NULL(surface);

    //SDL_Rect flipRect = {0, 0, surface->w, surface->h};
    //SDL_BlitSurface(surface, &flipRect, surface, &flipRect);
    // https://wiki.libsdl.org/SDL3/SDL_FlipMode
    //SDL_FlipSurface(surface, SDL_FLIP_HORIZONTAL);
    SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

    //SDL_Log("texture loadRcBitmapAsTexture");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    DeleteObject(hBitmap);
    //RETURN_IF_NULL(texture);

    //SDL_Log("return loadRcBitmapAsTexture");
    return texture;

    // this was one of the most painful functions to make in this project
}
#else
// context menu unsuported for other platforms, for now
void showContextMenu(struct AppState *state, int x, int y) {}
// is it fine that i use HWND even if its not defined?
// i dont even know what HWND is...
HWND getHwndFromWindow(SDL_Window *window) {}
// i dont even care about these definitions anymore
#endif

void drawFrame(struct AppState *state) {
    //SDL_Log("draw");
    int _bgColor = state->useBlackBg ? 0 : 0xff;
    SDL_SetRenderDrawColor(state->renderer, _bgColor, _bgColor, _bgColor, 0xff);
    SDL_RenderClear(state->renderer);

    calculateTextureRect(state);
    if (state->textureLoaded) {
        if (state->keepAspectRatio) {
            SDL_RenderTexture(state->renderer, state->texture, NULL, state->textureRect);
        } else {
            SDL_RenderTexture(state->renderer, state->texture, NULL, NULL);
        }
    } else {
        if (state->useBlackBg) {
            SDL_RenderTexture(state->renderer, state->welcomeBlack, NULL, state->textureRect);
        } else {
            SDL_RenderTexture(state->renderer, state->welcomeWhite, NULL, state->textureRect);
        }
    }

    SDL_RenderPresent(state->renderer);
    //SDL_Delay(1);
}

void handleEvent(struct AppState *state, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            state->quitApp = true;
            break;

        case SDL_EVENT_WINDOW_EXPOSED:
        case SDL_EVENT_WINDOW_RESIZED:
            state->needRedraw = true;
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
                    state->needRedraw = true;
                    break;

                case SDLK_B:
                    state->useBlackBg = !state->useBlackBg;
                    state->needRedraw = true;
                    break;

                case SDLK_T:
                    state->alwaysOnTop = !state->alwaysOnTop;
                    updateAlwaysOnTop(state);
                    break;

                case SDLK_L:
                    state->useAntialiasing = !state->useAntialiasing;
                    updateUseAntialiasing(state);
                    state->needRedraw = true;
                    break;

                case SDLK_R:
                    resizeWindowToImage(state);
                    break;

                case SDLK_C:
                    openImage(state, NULL);
                    break;

                case SDLK_O:
                    openImageWithDialog(state);
                    break;

                case SDLK_W:
                    state->keepWindowAspectRatio = !state->keepWindowAspectRatio;
                    updateKeepWindowAspectRatio(state);
                    break;

                case SDLK_S:
                    state->showImageName = !state->showImageName;
                    updateWindowTitle(state);
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
    state.textureLoaded = false;
    state.quitApp = false;
    state.needRedraw = false;
    state.dragging = false;
    state.dragX = 0;
    state.dragY = 0;

    // SETTINGS DEFAULTS
    state.keepAspectRatio = true;
    state.useBlackBg = false; // TODO: default to system's dark mode with 'SystemParametersInfo(SPI_GETCLIENTAREAOFWINDOW, 0, &is_dark_mode, 0)' inside 'windows.h'
    state.alwaysOnTop = true;
    state.useAntialiasing = true;
    state.keepWindowAspectRatio = false;
    state.showImageName = true;
    state.showImageName = false;
    readStateFromFile(&state);

    // imageName
    state.imageName = PS_Create(1);
    if (state.imageName == NULL) {
        SDL_Log("Memory allocation failed for imageName");
        return -1;
    }

    // sdl texture rect
    SDL_FRect _textureRect;
    state.textureRect = &_textureRect;

    // sdl init
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    // sdl window
    state.window = SDL_CreateWindow("plainIMG", W_WIDTH, W_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN); // SDL_WINDOW_RESIZABLE or 0
    if (state.window == NULL) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }
    SDL_SetWindowMinimumSize(state.window, WMIN_WIDTH, WMIN_HEIGHT);

    // sdl renderer
    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (state.renderer == NULL) {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -2;
    }
    SDL_SetEventFilter(&eventFilter, &state);

    // set background color to prevent flashing
    int _bgColor = state.useBlackBg ? 0 : 0xff;
    SDL_SetRenderDrawColor(state.renderer, _bgColor, _bgColor, _bgColor, 0xff);
    SDL_RenderClear(state.renderer);
    SDL_RenderPresent(state.renderer);
    SDL_ShowWindow(state.window);

    // textures
    // i can also just color a single white image, but im lazy, so i will have both for both light and dark mode
    //state.welcomeWhite = IMG_LoadTexture(state.renderer, WELCOME_WHITE_PATH);

    state.welcomeWhite = loadRcBitmapAsTexture(state.renderer, IDB_WELCOMEWHITE);
    if (state.welcomeWhite == NULL) {
        SDL_Log("loadRcBitmapAsTexture welcomeWhite error: %s", SDL_GetError());
        return -3;
    }
    state.welcomeBlack = loadRcBitmapAsTexture(state.renderer, IDB_WELCOMEBLACK);
    if (state.welcomeBlack == NULL) {
        SDL_Log("loadRcBitmapAsTexture welcomeBlack error: %s", SDL_GetError());
        return -4;
    }

    // other
    updateAlwaysOnTop(&state);
    updateUseAntialiasing(&state);
    updateKeepWindowAspectRatio(&state);
    // SDL_Log("SDL3 initialized");

    // ----- MAIN -----

    if (argc > 1) {
        openImage(&state, argv[1]);
    }

    state.needRedraw = true;
    SDL_Event event;
    while (!state.quitApp) {
        if (SDL_WaitEvent(&event)) {
            handleEvent(&state, &event);
        }
        if (state.needRedraw) {
            drawFrame(&state);
            state.needRedraw = false;
        }
    }

    // ----- QUIT -----

    // SDL_Log("SDL3 shutdown");
    writeStateToFile(&state);
    SDL_DestroyTexture(state.texture);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    // IMG_Quit();
    SDL_Quit();

    return 0;
}
