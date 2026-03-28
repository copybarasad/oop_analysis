#ifndef SCREENS
#define SCREENS

class GameScreen;
class PauseScreen;
class MainMenuScreen;
class TransitionToNewLevelScreen;

template <typename T>
class GameController;
template <typename T>
class PauseController;
template <typename T>
class MainMenuController;
template <typename T>
class TransitionToNewLevelController;

#include <memory>
using namespace std;

template <typename T>
class Screens
{
private:
    struct ScreensAndControllers
    {
        pair<shared_ptr<GameScreen>, shared_ptr<GameController<T>>> game;
        pair<shared_ptr<PauseScreen>, shared_ptr<PauseController<T>>> pause;
        pair<shared_ptr<MainMenuScreen>, shared_ptr<MainMenuController<T>>> mainMenu;
        pair<shared_ptr<TransitionToNewLevelScreen>, shared_ptr<TransitionToNewLevelController<T>>> newLvl;
    };

private:
    // static shared_ptr<GameScreen> mGame;
    // static shared_ptr<MainMenuScreen> mMainMenu;
    // static shared_ptr<PauseScreen> mPause;
    static Screens<T>::ScreensAndControllers mScreAndContr;

public:
    static void setGameScreen(shared_ptr<GameScreen> game)
    {
        mScreAndContr.game.first = game;
    }

    static void setGameController(shared_ptr<GameController<T>> game)
    {
        mScreAndContr.game.second = game;
    }

    static void setPauseScreen(shared_ptr<PauseScreen> pause)
    {
        mScreAndContr.pause.first = pause;
    }

    static void setPauseController(shared_ptr<PauseController<T>> pause)
    {
        mScreAndContr.pause.second = pause;
    }

    static void setMainMenuScreen(shared_ptr<MainMenuScreen> main)
    {
        mScreAndContr.mainMenu.first = main;
    }

    static void setMainMenuController(shared_ptr<MainMenuController<T>> main)
    {
        mScreAndContr.mainMenu.second = main;
    }

    static void setTransitionToNewLevelScreen(shared_ptr<TransitionToNewLevelScreen> main)
    {
        mScreAndContr.newLvl.first = main;
    }

    static void setTransitionToNewLevelController(shared_ptr<TransitionToNewLevelController<T>> main)
    {
        mScreAndContr.newLvl.second = main;
    }

    shared_ptr<GameScreen> getGameScreen()
    {
        return mScreAndContr.game.first;
    }

    shared_ptr<GameController<T>> getGameController()
    {
        return mScreAndContr.game.second;
    }

    pair<shared_ptr<GameScreen> *, shared_ptr<GameController<T>> *> getGame()
    {
        return pair<shared_ptr<GameScreen> *, shared_ptr<GameController<T>> *>(&mScreAndContr.game.first, &mScreAndContr.game.second);
    }

    shared_ptr<PauseScreen> getPauseScreen()
    {
        return mScreAndContr.pause.first;
    }

    shared_ptr<PauseController<T>> getPauseController()
    {
        return mScreAndContr.pause.second;
    }

    pair<shared_ptr<PauseScreen> *, shared_ptr<PauseController<T>> *> getPause()
    {
        return pair<shared_ptr<PauseScreen> *, shared_ptr<PauseController<T>> *>(&mScreAndContr.pause.first, &mScreAndContr.pause.second);
    }

    shared_ptr<MainMenuScreen> getMainMenuScreen()
    {
        return mScreAndContr.mainMenu.first;
    }

    shared_ptr<MainMenuController<T>> getMainMenuController()
    {
        return mScreAndContr.mainMenu.second;
    }

    shared_ptr<TransitionToNewLevelScreen> getTransitionToNewLevelScreen()
    {
        return mScreAndContr.newLvl.first;
    }

    shared_ptr<TransitionToNewLevelController<T>> getTransitionToNewLevelController()
    {
        return mScreAndContr.newLvl.second;
    }
};

template <typename T>
Screens<T>::ScreensAndControllers Screens<T>::mScreAndContr;

#endif