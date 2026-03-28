#ifndef RENDERER_INTERFACE_H
#define RENDERER_INTERFACE_H

#include "game.h"
#include "app_state.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    virtual void render(Game* game,
                       AppState state,
                       int cursorX,
                       int cursorY,
                       int spellIndex) = 0;
};

#endif