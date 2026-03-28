#ifndef BUILDING
#define BUILDING

#include "Entity.h"
#include "Enemy.h"
#include <memory>

class Building : public Entity
{
private:
    int mRangeForSpawn;
    int mCurentSpawn;

public:
    Building(const Building &b);

    Building(int maxLife, int rangeForSpawn);

    std::shared_ptr<Enemy> createNewEnemy();

    void doMove();

    void restoreCurentSpawn();

    bool isTimeForSpawn() const;

    Building &operator=(const Building &obj);
};

#endif