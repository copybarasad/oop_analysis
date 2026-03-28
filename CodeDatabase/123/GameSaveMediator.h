#ifndef I_GAME_SAVE_MEDIATOR
#define I_GAME_SAVE_MEDIATOR

#include "../SaveSystem/SaveSystem.h"
#include "Game.h"

// class Game;

class GameSaveMediator
{
private:
    // shared_ptr<SaveSystem> mSaveSystem;
    // shared_ptr<Game> mGame;

    shared_ptr<Map> *mMap;
    shared_ptr<Player> *mPlayer;
    shared_ptr<vector<shared_ptr<Enemy>>> mEnemies;
    shared_ptr<Building> *mBuilding;
    shared_ptr<size_t> mLevel;

    // protected:
    //     virtual void saveGame() = 0;
    //     virtual void loadFame() = 0;

public:
    void setComponent(Game *game)
    {
        // mGame = game;
        game->setMediator(this);
    }

    void setComponent(SaveSystem *save)
    {
        save->setData(SaveSystem::GameData(mMap, mPlayer, mEnemies.get(), mBuilding, mLevel.get()));
    }

    void setGameComponents(shared_ptr<Map> *map, shared_ptr<Player> *player,
                           shared_ptr<vector<shared_ptr<Enemy>>> enemies,
                           shared_ptr<Building> *building, shared_ptr<size_t> lvl)
    {
        mMap = map;
        mPlayer = player;
        mEnemies = enemies;
        mBuilding = building;
        mLevel = lvl;
    }
};

#endif