#pragma once
#include <string>
#include <tuple>

class Field;
class Player;

class EnemyTower {
private:
    int attackRadius_;
    int cooldown_;
    int currentCooldown_;
    int damage_;
    int hp_;   
    int maxHp_;  

public:
    EnemyTower(int radius = 4, int damage = 10, int cooldown = 2, int hp = 40);
    void tick();
    bool canAttack() const;
    void resetCooldown();
    bool attackPlayerIfInRange(Field& field, Player& player);
    int getDamage() const;
    
    int getHp() const;
    int getMaxHp() const;
    bool isAlive() const;
    void takeDamage(int damage);
    
    void setHp(int hp) { hp_ = hp; }
    void setDamage(int damage) { damage_ = damage; }
    void setCurrentCooldown(int cooldown) { currentCooldown_ = cooldown; }
    int getCurrentCooldown() const { return currentCooldown_; }
    int getCooldown() const { return cooldown_; }
    int getAttackRadius() const { return attackRadius_; }
    void setAttackRadius(int radius) { attackRadius_ = radius; }
    static std::tuple<int, int, int, int, int> parseData(const std::string& data);
};