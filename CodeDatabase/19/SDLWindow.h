#pragma once

#include <SDL.h>
#include <string>

class SDLWindow final  {
    friend class SDLRenderer;

public:
    ~SDLWindow();

    bool create(const std::string &title, int width, int height);
    void destroy();
    int width() const;
    int height() const;

private:
    SDL_Window *window_ = nullptr;
};
