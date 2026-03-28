
#include "SDLWindow.h"

#include <iostream>
#include <SDL_image.h>

bool SDLWindow::create(const std::string &title, const int width, const int height) {
    if (window_) {
        std::cerr << "SDL window already exists" << "\n";
        return false;
    }

    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );

    if (!window_) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
        return false;
    }


    SDL_Surface *icon = IMG_Load("assets/icon.png");
    if (icon) {
        SDL_SetWindowIcon(window_, icon);
        SDL_FreeSurface(icon);
    } else {
        std::cerr << "Failed to load icon: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

int SDLWindow::width() const {
    if (!window_) return 0;
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return w;
}

int SDLWindow::height() const {
    if (!window_) return 0;
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return h;
}


void SDLWindow::destroy() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

SDLWindow::~SDLWindow() {
    destroy();
}
