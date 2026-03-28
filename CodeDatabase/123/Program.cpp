#include "Program.h"

// Program::Program() : Game(), mSaveSys({&mMap, &mPlayer, &mEnemies, &mBuilding, &mLevel})
// {
// }

// template <typename RenderType>
// Program<RenderType>::Program()
// {
//     mGame = make_shared<Game>();
//     mSaveSys = make_shared<SaveSystem>();
//     mGameSave.setComponent(mGame);
//     // auto saveBuff = make_shared<SaveSystem>(mSaveSys);
//     mGameSave.setComponent(mSaveSys);
//     mRender.setGame(mGame);
//     mRender.setSaveSystem(mSaveSys);
// }

// template <typename RenderType>
// void Program<RenderType>::startProgram()
// {
//     mGame->newGame();
//     mRender.render();
// }

// template <typename RenderType>
// void Program<RenderType>::changeScreen(IScreen *scr)
// {
//     // delete mScreen;
//     // mScreen = scr;
//     // mScreen->setGame(this);
// }

// template <typename RenderType>
// void Program<RenderType>::start(RenderWindow &window, IScreen *screen)
// {
//     // mScreen = screen;
//     // mScreen->setGame(this);
//     // while (window.isOpen())
//     // {
//     //     mScreen->render(window);
//     // }
// }

// template <typename RenderType>
// void Program<RenderType>::saveGame()
// {
//     mSaveSys->save();
// }

// template <typename RenderType>
// void Program<RenderType>::loadGame()
// {
//     mSaveSys->load();
//     mGame->putEverythingOnTheMap();
// }

// template <typename RenderType>
// Program<RenderType>::~Program()
// {
//     // delete mScreen;
// }