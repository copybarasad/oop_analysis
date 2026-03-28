#ifndef GAME_SCREEN
#define GAME_SCREEN

#include "IScreen.h"
#include "../GameSystem/Game.h"
#include "../InputSystem/Controllers/LogicControllers/IController.h"

class GameScreen : public IScreen
{
protected:
    Game *mGame;
    // bool mIsMoveDone = false;
    // Map *mMap;
    // shared_ptr<IScreen> *mCurrentScreen;
    // Vec2 mHitPos;

public:
    GameScreen(Game *game /*, shared_ptr<IScreen> *current*/)
    {
        mGame = game;
        // mCurrentScreen = current;
    }

    void tick() override;

    void handleInput() override;

    // void render() override;
};

#endif