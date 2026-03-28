#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorAttributes.hpp"
#include "../actor_utility/ActorAttack.hpp"
#include "../actor_utility/ActorEquipment.hpp"
#include "../actor_utility/ActorHealth.hpp"
#include "../core/Config.hpp"
#include "../core/SaveData.hpp"
#include <utility>
#include <memory>


class Enemy : public Entity {
private:
    Attack enemyAttack;
    Health enemyHealth;
    int enemyLevel;
public:
    Enemy();
    Enemy(int enemyLevel);
    entityType getType() const override {
        return entityType::ENEMY;
    }
    int getDamage() override;
    int getLevel() override;
    std::pair<int, int>  getHealth() override;
    void causeDamage(int damage) override;
    bool alive() const override;
    char returnEntitySymbol() override;
    EnemySaveData getEnemySaveData();
    void setEnemySaveData(EnemySaveData data);
};
