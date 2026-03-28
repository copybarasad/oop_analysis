#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Building.hpp"
#include "ISpell.hpp"
#include <memory>
#include <iostream>

class GameState;
class SpellCaster;

class EnemyTower : public Building {
private:
    int attackRange_;
    int attackCooldown_;
    int currentAttackCooldown_;
    std::unique_ptr<ISpell> attackSpell_;

    std::unique_ptr<ISpell> createAttackSpell();

public:
    EnemyTower(int x, int y, int health = 40, int attackRange = 3, int attackCooldown = 3);
    
    void update() override;
    bool isActive() const override;
    
    bool canAttack() const;
    bool tryAttack(int targetX, int targetY, GameState& gameState);
    int getAttackRange() const;

    void serialize(std::ostream& os) const override {
        Building::serialize(os);
        os.write(reinterpret_cast<const char*>(&attackRange_), sizeof(attackRange_));
        os.write(reinterpret_cast<const char*>(&attackSpell_), sizeof(attackSpell_));
        os.write(reinterpret_cast<const char*>(&attackCooldown_), sizeof(attackCooldown_));
        os.write(reinterpret_cast<const char*>(&currentAttackCooldown_), sizeof(currentAttackCooldown_));
    }
    
    void deserialize(std::istream& is) override {
        Building::deserialize(is);
        is.read(reinterpret_cast<char*>(&attackRange_), sizeof(attackRange_));
        is.read(reinterpret_cast<char*>(&attackSpell_), sizeof(attackSpell_));
        is.read(reinterpret_cast<char*>(&attackCooldown_), sizeof(attackCooldown_));
        is.read(reinterpret_cast<char*>(&currentAttackCooldown_), sizeof(currentAttackCooldown_));
    }
};

#endif