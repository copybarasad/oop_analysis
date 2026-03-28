#include "SaveSystem.h"

// SaveSystem::SaveSystem(GameData dat)
// {
// }

void SaveSystem::setData(const GameData dat)
{
    mOldData = dat;
    createSaves(mSaveVec, mOldData);
}

void SaveSystem::save()
{
    fstream file;
    file.open("save.sav", ios::binary | ios::out);
    if (!file.is_open())
    {
        throw std::logic_error("Cannot open save file");
    }

    for (size_t i = 0; i < mSaveVec.size(); i++)
    {
        mSaveVec[i]->write(file);
    }

    file.close();
}

void SaveSystem::load()
{
    fstream file;
    file.open("save.sav", ios::binary | ios::in);
    if (!file.is_open())
    {
        throw std::logic_error("Cannot open save file");
    }
    GameData newData;
    newData.mMap = new shared_ptr<Map>();
    newData.mPlayer = new shared_ptr<Player>;
    newData.mEnemyVec = new vector<shared_ptr<Enemy>>;
    newData.mBuilding = new shared_ptr<Building>;
    newData.mLevel = new size_t;
    vector<ISave *> mLoadVec;
    createSaves(mLoadVec, newData);

    for (size_t i = 0; i < mSaveVec.size(); i++)
    {
        mLoadVec[i]->read(file);
        delete mLoadVec[i];
    }

    if (file.eof() && file.fail())
    {
        file.close();
        throw runtime_error("File is broken");
    }

    file.close();

    *mOldData.mMap = *newData.mMap;
    *mOldData.mPlayer = *newData.mPlayer;
    mOldData.mEnemyVec->clear();
    for (size_t i = 0; i < newData.mEnemyVec->size(); i++)
    {
        mOldData.mEnemyVec->push_back(make_shared<Enemy>(*newData.mEnemyVec->at(i).get()));
    }
    *mOldData.mBuilding = *newData.mBuilding;
    *mOldData.mLevel = *newData.mLevel;

    delete newData.mMap;
    delete newData.mPlayer;
    delete newData.mEnemyVec;
    delete newData.mBuilding;
    delete newData.mLevel;
}

SaveSystem::~SaveSystem()
{
    for (size_t i = 0; i < mSaveVec.size(); i++)
    {
        delete mSaveVec[i];
    }
}

void SaveSystem::createSaves(vector<ISave *> &mVec, GameData &dat)
{
    mVec = {
        new CheckSave(),
        new MapSave(dat.mMap),
        new PlayerSave(dat.mPlayer),
        new EnemyVectorSave(dat.mEnemyVec),
        new BuldingSave(dat.mBuilding),
        new LevelSave(dat.mLevel)};
}