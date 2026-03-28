#include "Hero.h"
#include <iostream>
#include "Foe.h"  
#include "EnemyTower.h" 
#include "GameBoard.h"

Hero::Hero(int hitpoints, int meleeDmg, int rangedDmg, const Location& pos)
    : GameActor(hitpoints, meleeDmg, pos), 
      meleeModeEnabled(true), 
      meleeDamage(meleeDmg), 
      rangedDamage(rangedDmg),
      spellHand(5, 0),
      enemiesDefeated(0) {
}

void Hero::performMove(MoveDirection dir, GameBoard& board) {
    Location newPos = position;
    
    switch (dir) {
        case MoveDirection::North:
            if (newPos.row > 0) newPos.row--;
            break;
        case MoveDirection::South:
            if (newPos.row < board.getWidth() - 1) newPos.row++;
            break;
        case MoveDirection::West:
            if (newPos.col > 0) newPos.col--;
            break;
        case MoveDirection::East:
            if (newPos.col < board.getHeight() - 1) newPos.col++;
            break;
    }
    
    if (board.isMoveValid(position, newPos, this)) {
        board.moveActor(position, newPos, this);
        displayMove(newPos);
    } else {
        std::cout << "Hero is blocked by another actor\n";
    }
}

void Hero::handleCollision(GameActor* other) {
    if (isAlive() && other && other->isAlive()) {
        if (meleeModeEnabled) {
            std::cout << "Hero melee attack! Damage: " << meleeDamage << "\n";
            other->receiveDamage(meleeDamage);
            
            // Враг тоже атакует в ответ
            if (other->isAlive()) {
                std::cout << "Enemy counter-attack! Damage: " << other->getAttackPower() << "\n";
                receiveDamage(other->getAttackPower());
            }
        } else {
            std::cout << "Hero ranged attack in melee! Damage: " << rangedDamage << "\n";
            other->receiveDamage(rangedDamage);
        }
    }
}
void Hero::toggleCombatStyle() {
    meleeModeEnabled = !meleeModeEnabled;
    std::cout << "Combat style: " << (meleeModeEnabled ? "Melee" : "Ranged") << "\n";
}

int Hero::getCurrentDamage() const {
    return meleeModeEnabled ? meleeDamage : rangedDamage;
}

bool Hero::isMeleeMode() const {
    return meleeModeEnabled;
}

bool Hero::performRangedAttack(GameBoard& board) {
    if (isMeleeMode()) {
        std::cout << "Switch to ranged mode first with 'c'!\n";
        return false;
    }
    
    // Ищем врагов И башни в пределах досягаемости
    bool attacked = false;
    for (size_t row = 0; row < board.getWidth(); ++row) {
        for (size_t col = 0; col < board.getHeight(); ++col) {
            Location targetLoc(row, col);
            int distance = board.calculateDistance(getPosition(), targetLoc);
            
            // Дальняя атака работает на расстоянии 2-5 клеток
            if (distance >= 2 && distance <= 5) {
                GameActor* target = board.getActorAt(targetLoc);
                if (target && target->isAlive()) {
                    // Проверяем, является ли цель врагом ИЛИ башней
                    if (dynamic_cast<Foe*>(target) || dynamic_cast<EnemyTower*>(target)) {
                        std::cout << "Hero ranged attack! Hit target at (" 
                                  << targetLoc.row << "," << targetLoc.col 
                                  << ") Damage: " << rangedDamage 
                                  << " Target HP: " << target->getHitpoints() - rangedDamage 
                                  << "/" << target->getMaxHitpoints() << "\n";
                        target->receiveDamage(rangedDamage);
                        attacked = true;
                        
                        // Если цель была врагом, проверяем на смерть для награды маны
                        if (dynamic_cast<Foe*>(target) && !target->isAlive()) {
                            int manaReward = 45;
                            spellHand.addMana(manaReward);
                            std::cout << "Enemy defeated by ranged attack! +" << manaReward << " mana!\n";
                            incrementEnemiesDefeated();
                        }
                    }
                }
            }
        }
    }
    
    if (!attacked) {
        std::cout << "No valid targets found for ranged attack (need distance 2-5)!\n";
    }
    
    return attacked;
}

void Hero::castSpell(int spellIndex, const Location& target, GameBoard& board) {
    std::cout << "DEBUG: Starting castSpell, index=" << spellIndex 
              << ", target=(" << target.row << "," << target.col << ")\n";
    
    if (spellIndex < 0 || spellIndex >= static_cast<int>(spellHand.getSpellCount())) {
        std::cout << "DEBUG: Invalid spell index\n";
        std::cout << "Invalid spell selection!\n";
        return;
    }
    
    const Spell* spell = spellHand.getSpell(spellIndex);
    if (!spell) {
        std::cout << "DEBUG: Spell not found\n";
        std::cout << "Spell not found!\n";
        return;
    }
    
    std::cout << "DEBUG: Spell found: " << spell->getName() << "\n";
    
    // Кастуем заклинание
    std::cout << "DEBUG: Calling spellHand.castSpell()\n";
    if (spellHand.castSpell(spellIndex, getPosition(), target, board)) {
        std::cout << "DEBUG: Spell cast successful\n";
        std::cout << "Casting " << spell->getName() << " at (" << target.row << "," << target.col << ")...\n";
    } else {
        std::cout << "DEBUG: Spell cast failed\n";
        std::cout << "Failed to cast " << spell->getName() << "!\n";
    }
    
    std::cout << "DEBUG: castSpell method completed\n";
}

void Hero::displayMove(const Location& newPos) const {
    std::cout << "Hero moved to (" << newPos.row << "," << newPos.col << ")\n";
}