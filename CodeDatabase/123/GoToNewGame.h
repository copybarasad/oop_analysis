#ifndef NEW_GAME
#define NEW_GAME

#include "ICommand.h"
#include "GoToGame.h"

template <typename T>
class GoToNewGame : public GoToGame<T>
{
private:
    // Game *mGame;

public:
    // GoToNewGame(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : GoToGame<T>(screen, controller, game)
    // {
    //     // mGame = game;
    // }
    using GoToGame<T>::GoToGame;

    bool execute() override
    {
        this->mGame->newGame();
        return GoToGame<T>::execute();
    }
};

#endif