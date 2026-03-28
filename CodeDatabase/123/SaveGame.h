#ifndef SAVE_GAME
#define SAVE_GAME

#include "ICommand.h"
#include "../../SaveSystem/SaveSystem.h"
#include "../../GameSystem/GameSaveMediator.h"

class SaveGame : public ICommand
{
private:
    SaveSystem mSave;
    GameSaveMediator mMediator;

public:
    SaveGame(Game *game)
    {
        mMediator.setComponent(game);
        mMediator.setComponent(&mSave);
    }

    bool execute() override
    {
        try
        {
            mSave.save();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return true;
    }
};

#endif