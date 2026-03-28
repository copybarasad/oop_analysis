#ifndef SAVE_SYSTEM
#define SAVE_SYSTEM

#include "CheckSave.h"
#include "MapSave.h"
#include "PlayerSave.h"
#include "EnemyVectorSave.h"
#include "BuildingSave.h"
#include "LevelSave.h"
#include "fstream"

class SaveSystem
{
public:
    struct GameData
    {
        shared_ptr<Map> *mMap;
        shared_ptr<Player> *mPlayer;
        vector<shared_ptr<Enemy>> *mEnemyVec;
        shared_ptr<Building> *mBuilding;
        size_t *mLevel;

        GameData() {}

        GameData(shared_ptr<Map> *map,
                 shared_ptr<Player> *player,
                 vector<shared_ptr<Enemy>> *enemyVec,
                 shared_ptr<Building> *building,
                 size_t *level) : mMap(map), mPlayer(player), mEnemyVec(enemyVec), mBuilding(building), mLevel(level)
        {
        }
    };

private:
    vector<ISave *> mSaveVec;
    GameData mOldData;

public:
    // SaveSystem(GameData dat);

    void setData(const GameData dat);

    void save();

    void load();

    ~SaveSystem();

private:
    void createSaves(vector<ISave *> &mVec, GameData &dat);
};

#endif