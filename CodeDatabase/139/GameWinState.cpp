#include "GameWinState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Commands/Converters/GameWinConverter.h"
#include "Rendering/IRenderer.h"


GameWinState::GameWinState(Game* game) : game(game) { converter = new GameWinConverter(game); }


void GameWinState::update() {}


void GameWinState::render(IRenderer& renderer) { renderer.renderGameWin(); }


IConverter* GameWinState::getConverter() { return converter; }


GameWinState::~GameWinState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}