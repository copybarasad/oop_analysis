#include "GameOverState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Commands/Converters/GameOverConverter.h"
#include "Rendering/IRenderer.h"


GameOverState::GameOverState(Game* game) : game(game) { converter = new GameOverConverter(game); }


void GameOverState::update() {}


void GameOverState::render(IRenderer& renderer) { renderer.renderGameOver(); }


IConverter* GameOverState::getConverter() { return converter; }


GameOverState::~GameOverState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}