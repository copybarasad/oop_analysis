#include "PauseState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Commands/Converters/PauseConverter.h"
#include "Rendering/IRenderer.h"
#include <iostream>


PauseState::PauseState(Game* game) : game(game) { converter = new PauseConverter(game); }


void PauseState::update() {}


void PauseState::render(IRenderer& renderer) { renderer.renderPause(); }


IConverter* PauseState::getConverter() { return converter; }


PauseState::~PauseState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}