#ifndef ENEMYSERIALIZER_H
#define ENEMYSERIALIZER_H

#include "ISerializer.h"
#include "EntitySerializer.h"
#include "../entities/Enemy.h"

class EnemySerializer : public ISerializer<Enemy> {
private:
    EntitySerializer entitySerializer;
    
public:
    nlohmann::json serialize(const Enemy& enemy) const override;
    void deserialize(Enemy& enemy, const nlohmann::json& json) const override;
};

#endif

