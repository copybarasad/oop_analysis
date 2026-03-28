#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerHand.h"
#include "Position.h"

class GameField;

// Класс игрока - управляет здоровьем, маной и заклинаниями
class Player {
private:
    Position position_;
    PlayerHand hand_;
    int health_;
    int max_health_;
    int mana_;
    int max_mana_;

public:
    Player(int start_x, int start_y, int health, int mana);

    // Основные действия игрока
    bool castSpell(size_t spell_index, GameField& field, const SpellCastInfo& cast_info);
    bool move(int target_x, int target_y, GameField& field);
    void takeDamage(int damage);
    void addMana(int amount);
    void restoreMana(int amount);
    bool canAffordSpell(int mana_cost) const;
    void spendMana(int amount);

    // Геттеры для характеристик
    const Position& getPosition() const { return position_; }
    const PlayerHand& getHand() const { return hand_; }
    PlayerHand& getHand() { return hand_; }
    int getHealth() const { return health_; }
    int getMana() const { return mana_; }
    int getMaxMana() const { return max_mana_; }
    bool isAlive() const { return health_ > 0; }

    // Сеттеры для позиции и характеристик
    void setPosition(int x, int y);
    void setMaxHealth(int max_health);
    void setMaxMana(int max_mana);
    void restoreHealthToMax();
    void restoreManaToMax();
    void setHealth(int health) {
        health_ = health;
        if (health_ < 0) health_ = 0;
        if (health_ > max_health_) health_ = max_health_;
    }

    void setMana(int mana) {
        mana_ = mana;
        if (mana_ < 0) mana_ = 0;
        if (mana_ > max_mana_) mana_ = max_mana_;
    }

    void clearHand() {
        // Создаем новую пустую руку
        hand_ = PlayerHand();
    }

    int getMaxHealth() const { return max_health_; }
};

#endif