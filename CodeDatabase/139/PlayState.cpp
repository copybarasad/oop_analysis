#include "PlayState.h"
#include "LevelWinState.h"
#include "GameWinState.h"
#include "GameOverState.h"
#include "Commands/Converters/PlayConverter.h"
#include "Rendering/IRenderer.h"
#include "Interfaces/Game.h"
#include <iostream>


PlayState::PlayState(Game* game) : game(game) { converter = new PlayConverter(game); }


void PlayState::update() {
    if (game->getExecuteUseCase()->checkGameEnd())
    {
        if (game->getExecuteUseCase()->checkGameWin())
        {
            if (game->getGameLevel() == game->getGameLevelMax())
            {
                std::cout << u8"Переход в игра пройдена GameWinState\n";
                game->setState(new GameWinState(game));
            }
            else {
                game->setState(new LevelWinState(game));
            }

            return;
        }

        if (game->getExecuteUseCase()->checkGameOver())
        {
            std::cout << u8"Переход в игра провалена GameOverState\n";
            game->setState(new GameOverState(game));
        }
    }
}


void PlayState::render(IRenderer& renderer) { renderer.renderMap(); }


IConverter* PlayState::getConverter() { return converter; }


PlayState::~PlayState()
{
    if (converter != nullptr)
    {
        delete converter;
        converter = nullptr;
    }
}