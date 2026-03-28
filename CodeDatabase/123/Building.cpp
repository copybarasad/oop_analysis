#include "Building.h"
#include <random>
#include <ctime>
#include <math.h>

Building::Building(const Building &b) : Entity(b)
{
    mRangeForSpawn = b.mRangeForSpawn;
    mCurentSpawn = b.mCurentSpawn;
}

Building::Building(int maxLife, int rangeForSpawn) : Entity(maxLife), mRangeForSpawn(rangeForSpawn)
{
    mType = CharacterType::Building;
    mCurentSpawn = rangeForSpawn;
    mExpForKill = 20;
}

std::shared_ptr<Enemy> Building::createNewEnemy()
{
    // srand(1);
    // Vec2 shift(1, 1);
    // while (shift != Vec2(0, 0))
    // {
    //     std::random_device rseed;
    //     std::mt19937 rng(rseed());
    //     std::uniform_int_distribution<int> dist(-2, 2);
    //     // shift = Vec2(-2 + rand() % 5, -2 + rand() % 5);
    //     shift = Vec2(dist(rng), dist(rng));
    // }

    // Enemy *en = new Enemy(10, 5, 3);
    // en->setPosition(getPosition() + shift);
    return std::make_shared<Enemy>(10, 5, 3);
}

void Building::doMove()
{
    mCurentSpawn = std::max(0, mCurentSpawn - 1);
}

void Building::restoreCurentSpawn()
{
    mCurentSpawn = mRangeForSpawn;
}

bool Building::isTimeForSpawn() const
{
    return mCurentSpawn == 0;
}

Building &Building::operator=(const Building &obj)
{
    Building tmp(obj);

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;
    mExpForKill = tmp.mExpForKill;

    mRangeForSpawn = tmp.mRangeForSpawn;
    mCurentSpawn = tmp.mCurentSpawn;

    return *this;
}
