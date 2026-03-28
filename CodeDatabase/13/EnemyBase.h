#ifndef GAME_ENEMYBASE_H
#define GAME_ENEMYBASE_H

#include  "../basics/Object.h"

class EnemyBase : public Object {
private:
    int cooldown_;
    int counter_;

public:
    explicit EnemyBase(const int cooldown = 5) : Object("EnemyBase", ObjTypes::Neither),
                                                 cooldown_(cooldown), counter_(cooldown) {
    };

    void spawnEnemy();

    void update() override;

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
