#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorHealth.hpp"
#include "../items/DirectDamageSpell.hpp"
#include "../core/Config.hpp"
#include "../core/SaveData.hpp"

class EnemyTower : public Entity{
private:
    Health towerHealth{};
    DirectDamageSpell towerSpell{};
    int attackPeriod;
    int attackCooldown;
    int towerlevel;
public:
    entityType getType() const override {
        return entityType::TOWER;
    }
    void causeDamage(int damage) override;
    std::pair<int, int>  getHealth() override;
    bool alive() const override;
    EnemyTower();
    EnemyTower(int towerLevel);
    EnemyTower(int towerLevel, int spellDamage, int spellDistance, int attackPeriod);
    int getDamage() override;
    void towerAttack(GameContext &ctx, int userIndex, int power);
    int getSpellDistance();
    int getLevel() override;
    int getInt() override;
    char returnEntitySymbol() override;
    TowerSaveData getTowerSaveData();
    void setTowerSaveData(TowerSaveData data);
};