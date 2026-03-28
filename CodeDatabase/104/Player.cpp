#include <iostream>
#include "Player.h"
#include "GameField.h"

// Конструктор игрока
Player::Player(int start_x, int start_y, int health, int mana)
    : position_(start_x, start_y),
    health_(health), max_health_(health),
    mana_(mana), max_mana_(mana) {
}

// Применение заклинания
bool Player::castSpell(size_t spell_index, GameField& field, const SpellCastInfo& cast_info) {
    SpellCard* spell = hand_.getSpell(spell_index);
    if (!spell) {
        return false;
    }
    return spell->cast(*this, field, cast_info);
}

// Перемещение игрока
bool Player::move(int target_x, int target_y, GameField& field) {
    return field.movePlayer(target_x, target_y);
}

// Получение урона
void Player::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}

// Добавление маны
void Player::addMana(int amount) {
    mana_ += amount;
    if (mana_ > max_mana_) {
        mana_ = max_mana_;
    }
}

// Восстановление маны
void Player::restoreMana(int amount) {
    mana_ += amount;
    if (mana_ > max_mana_) {
        mana_ = max_mana_;
    }
}

// Проверка достаточности маны
bool Player::canAffordSpell(int mana_cost) const {
    return mana_ >= mana_cost;
}

// Трата маны
void Player::spendMana(int amount) {
    mana_ -= amount;
    if (mana_ < 0) {
        mana_ = 0;
    }
}

// Установка позиции
void Player::setPosition(int x, int y) {
    position_.setX(x);
    position_.setY(y);
}

// Установка максимального здоровья
void Player::setMaxHealth(int max_health) {
    max_health_ = max_health;
    if (health_ > max_health_) {
        health_ = max_health_;
    }
}

// Установка максимальной маны
void Player::setMaxMana(int max_mana) {
    max_mana_ = max_mana;
    if (mana_ > max_mana_) {
        mana_ = max_mana_;
    }
}

// Полное восстановление здоровья
void Player::restoreHealthToMax() {
    health_ = max_health_;
    std::cout << "Health fully restored to " << max_health_ << "!\n";
}

// Полное восстановление маны
void Player::restoreManaToMax() {
    mana_ = max_mana_;
    std::cout << "Mana fully restored to " << max_mana_ << "!\n";
}