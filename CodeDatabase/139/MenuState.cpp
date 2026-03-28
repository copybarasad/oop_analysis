#include "MenuState.h"
#include "PlayState.h"
#include "Commands/Converters/MenuConverter.h"
#include "Rendering/IRenderer.h"


MenuState::MenuState(Game* game) : game(game) { converter = new MenuConverter(game); }


void MenuState::update() {}


void MenuState::render(IRenderer& renderer) { renderer.renderMenu(); }


IConverter* MenuState::getConverter() { return converter; }


MenuState::~MenuState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}