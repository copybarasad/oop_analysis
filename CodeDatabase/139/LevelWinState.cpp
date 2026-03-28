#include "LevelWinState.h"
#include "LevelBetweenState.h"
#include "MenuState.h"
#include "Commands/Converters/LevelWinConverter.h"
#include "Rendering/IRenderer.h"


LevelWinState::LevelWinState(Game* game) : game(game) { converter = new LevelWinConverter(game); }


void LevelWinState::update() {}


void LevelWinState::render(IRenderer& renderer) { renderer.renderLevelWin(); }


IConverter* LevelWinState::getConverter() { return converter; }


LevelWinState::~LevelWinState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}