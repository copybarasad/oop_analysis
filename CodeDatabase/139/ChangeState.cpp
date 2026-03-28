#include "ChangeState.h"
#include "Interfaces/Game.h"
#include "States/MenuState.h"
#include "States/PlayState.h"
#include "States/PauseState.h"
#include "States/LevelWinState.h"
#include "States/LevelBetweenState.h"
#include "States/GameOverState.h"
#include "States/GameWinState.h"


ChangeState::ChangeState(Game* game, State state) : game(game), state(state) {}


void ChangeState::execute()
{
    switch (state)
    {
    case State::MENU:           game->setState(new MenuState(game)); break;
    case State::PLAY:           game->setState(new PlayState(game)); break;
    case State::PAUSE:          game->setState(new PauseState(game)); break;
    case State::LEVELWIN:       game->setState(new LevelWinState(game)); break;
    case State::LEVELBETWEEN:   game->setState(new LevelBetweenState(game)); break;
    case State::GAMEWIN:        game->setState(new GameWinState(game)); break;
    case State::GAMEOVER:       game->setState(new GameOverState(game)); break;
    default: break;
    }
}
