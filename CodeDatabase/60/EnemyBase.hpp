#pragma once
#include <LivingEntity.hpp>
#include <Enemy.hpp>

class EnemyBase : public LivingEntity {
public:
    EnemyBase(int x, int y, int level);

    void spawnEnemy(std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen, int height, int width);
    virtual void draw() override;
    void destroyed();
    int getExp() const;
    int getLvl() const;
    
private:
    char sprite = ENEMY_BASE;
    int level;
};