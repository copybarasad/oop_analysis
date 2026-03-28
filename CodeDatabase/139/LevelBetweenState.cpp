#include "LevelBetweenState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Commands/Converters/LevelBetweenConverter.h"
#include "Rendering/IRenderer.h"


LevelBetweenState::LevelBetweenState(Game* game) : game(game) { converter = new LevelBetweenConverter(game); }


void LevelBetweenState::update() {}


void LevelBetweenState::render(IRenderer& renderer) { renderer.renderLevelBetween(); }


IConverter* LevelBetweenState::getConverter() { return converter; }


LevelBetweenState::~LevelBetweenState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}