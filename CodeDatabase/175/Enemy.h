#pragma once
#include <string>
#include <utility>

class Enemy {
private:
    int hp_;
    int maxHp_;
    int damage_;

public:
    Enemy(int hp = 30, int damage = 5);
    int getHp() const;
    int getMaxHp() const;
    int getDamage() const;
    bool isAlive() const;
    void takeDamage(int dmg);
    
    void setHp(int hp) { hp_ = hp; }
    void setDamage(int damage) { damage_ = damage; }
    static std::pair<int, int> parseData(const std::string& data);
};