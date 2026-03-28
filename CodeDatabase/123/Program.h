#ifndef PROGRAM
#define PROGRAM

#include "GameSystem/Game.h"
#include "HasFont.h"
#include "Screens/IScreen.h"
#include "SaveSystem/SaveSystem.h"

// #include "InputSystem/IController.h"
// #include "InputSystem/SFMLController.h"
// #include "InputSystem/Controllers/SFMLGameController.h"
#include "Renderers/IRender.h"
#include "GameSystem/GameSaveMediator.h"

template <typename T>
concept RenderConcept = requires(T render) {
    { render.setGame(shared_ptr<Game>()) };
    { render.getCurrentScreen() } -> std::same_as<shared_ptr<IScreen> *>;
    { render.isEnd() };
    { render.start() };
    // { controller.handleInput() };
};
// template <typename T>
// concept RenderConcept = convertible_to<T, IRender>;

template <typename T>
concept ControllerConcept = requires(T controller) {
    // { render.getCurrentScreen() };
    { controller.handleInput() };
    { T(new shared_ptr<IScreen>(), new Game()) };
};

template <RenderConcept RenderType, ControllerConcept Controller>
class Program
{
protected:
    shared_ptr<Game> mGame;
    RenderType mRender;
    shared_ptr<Controller> mController;

public:
    Program()
    {
        mGame = make_shared<Game>();
        // mSaveSys = make_shared<SaveSystem>();
        // mGameSave.setComponent(mGame);
        // mGameSave.setComponent(mSaveSys);
        // mGame->newGame();
        mRender.setGame(mGame);
        mController = make_shared<Controller>(mRender.getCurrentScreen(), mGame.get());
        // mRender.setSaveSystem(mSaveSys);
    }

    // void setRenderType(shared_ptr<IRender> render);

    void startProgram()
    {
        // mGame->newGame();
        mRender.start();
        while (!mRender.isEnd())
        {
            mController->handleInput();
            mRender.getCurrentScreen()->get()->tick();
            mRender.getCurrentScreen()->get()->render();
            // mRender.getCurrentScreen()->tick();
            // mRender.getCurrentScreen()->render();
        }
    }

    // void changeScreen(IScreen *);

    // void start(RenderWindow &, IScreen *);

    // void saveGame();

    // void loadGame();

    // ~Program();
};

#endif