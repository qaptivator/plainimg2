#ifndef PLAINIMG_UTILS_H
#define PLAINIMG_UTILS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

static inline int setWindowAlwaysOnTop(SDL_Window* window, bool enable);

#ifdef _WIN32

    #include <windows.h>
    static inline int setWindowAlwaysOnTop(SDL_Window* window, bool enable) {
        //HWND hwnd = SDL_GetWindowWMInfo(window)->info.win.window;
        //HWND hwnd = SDL_GetWindowProperties(window)
        //if (enable) {
        //    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        //} else {
        //    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        //}
        //return 0;
        return -1;
    }

#elif defined(__APPLE__)

    // NOT IMPLEMENTED YET
    //#include <Cocoa/Cocoa.h>
    static inline int setWindowAlwaysOnTop(SDL_Window* window, bool enable) {
        return -1;
    }

#elif defined(__linux__)

    // NOT IMPLEMENTED YET
    //#include <X11/Xlib.h>
    static inline int setWindowAlwaysOnTop(SDL_Window* window, bool enable) {
        return -1;
    }

#endif

#endif