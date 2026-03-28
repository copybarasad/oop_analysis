#pragma once
#include "../../gamecore/include/Position.h"
#include "Entity.h"
#include <memory>

// Здание знает только свой интервал и может производить врагов.
// Размещение (spawnNearby) выполняет менеджер.
class EnemyBuilding : public Entity {
public:
    // hp - здоровье здания, spawnInterval - период спавна врагов
    EnemyBuilding(int hp, int spawnInterval, int spawnHp, int spawnDmg);
    
    int spawnInterval() const;
    int hp() const { return hp_; }
    // Инкрементирует внутренний счетчик; когда достигает периода — возвращает true и сбрасывает
    bool timeToSpawn();
    std::unique_ptr<Entity> spawnEnemy() const;

    EntityType type() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    int spawnInterval_;
    int counter_{0};
    int spawnHp_;
    int spawnDmg_;
};
