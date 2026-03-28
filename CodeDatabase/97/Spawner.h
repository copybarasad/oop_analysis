#ifndef UNTITLED_SPAWNER_H
#define UNTITLED_SPAWNER_H

#include "Entity.h"

class Spawner : public Entity{
public:
    bool readyToSpawn() const;
    void resetTimer();
    ~Spawner() override = default;
    Spawner(int hp = 10, int spawnInterval = 3);
    std::shared_ptr<Entity> clone() const override;
    void onTurn(Field& field, const Coord& pos) override;
    void decrementTimer();

    void setTimer(int t);
    int timer() const;

private:
    int spawnInterval_;
    int timer_;
};

#endif //UNTITLED_SPAWNER_H

