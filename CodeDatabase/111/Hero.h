#pragma once
#include "GameActor.h"
#include "MoveDirection.h"
#include "SpellHand.h"

enum class MoveDirection;

class Hero : public GameActor {
private:
    bool meleeModeEnabled;
    int meleeDamage;
    int rangedDamage;
    SpellHand spellHand;
    int enemiesDefeated;

public:
    Hero(int hitpoints, int meleeDmg, int rangedDmg, const Location& pos);
    
    // Удаляем только копирование, разрешаем перемещение
    Hero(const Hero&) = delete;
    Hero& operator=(const Hero&) = delete;
    
    // Разрешаем перемещение
    Hero(Hero&& other) noexcept
        : GameActor(std::move(other)),
          meleeModeEnabled(other.meleeModeEnabled),
          meleeDamage(other.meleeDamage),
          rangedDamage(other.rangedDamage),
          spellHand(std::move(other.spellHand)),
          enemiesDefeated(other.enemiesDefeated) {
    }
    
    Hero& operator=(Hero&& other) noexcept {
        if (this != &other) {
            GameActor::operator=(std::move(other));
            meleeModeEnabled = other.meleeModeEnabled;
            meleeDamage = other.meleeDamage;
            rangedDamage = other.rangedDamage;
            spellHand = std::move(other.spellHand);
            enemiesDefeated = other.enemiesDefeated;
        }
        return *this;
    }
    
    void performMove(MoveDirection dir, GameBoard& board) override;
    void handleCollision(GameActor* other) override;
    
    void toggleCombatStyle();
    int getCurrentDamage() const;
    bool isMeleeMode() const;
    bool performRangedAttack(GameBoard& board);
    
    void castSpell(int spellIndex, const Location& target, GameBoard& board);
    void incrementEnemiesDefeated() { enemiesDefeated++; }
    
    const SpellHand& getSpellHand() const { return spellHand; }
    SpellHand& getSpellHand() { return spellHand; }
    int getEnemiesDefeated() const { return enemiesDefeated; }
    
    void setMana(int mana) { spellHand.setMana(mana); }
    
    void reset() {
        attributes.hitpoints = attributes.getMaxHitpoints();
        enemiesDefeated = 0;
        // Не можем сбросить spellHand напрямую, так как это unique_ptr
        // Вместо этого очистим заклинания
        while (spellHand.getSpellCount() > 0) {
            spellHand.removeSpell(0);
        }
        spellHand.setMana(0);
    }
    
private:
    void displayMove(const Location& newPos) const;
};