#include "Player.h"


// Реализация логики игрока
// Содержит методы управления игроком, использует конфигурацию
// для балансировки характеристик. Исправлена логика замедления.


// Конструктор по умолчанию - использует стандартную конфигурацию
Player::Player() 
    : Entity(100, Position(0, 0), "Player", 0),  // Значения по умолчанию
      damage(5), score(0), canAct(true), slowed(false),
      battleMode(BattleMode::CLOSE), hand() {}

// Конструктор с начальной позицией - использует стандартную конфигурацию
Player::Player(const Position& startPos) 
    : Entity(100, startPos, "Player", 0),
      damage(5), score(0), canAct(true), slowed(false),
      battleMode(BattleMode::CLOSE), hand() {}

// Конструктор с конфигурацией - использует переданные настройки
Player::Player(const Position& startPos, const PlayerConfig& config)
    : Entity(config.startHealth, startPos, "Player", 0),  // Используем конфигурацию
      damage(config.baseDamage), 
      score(config.startScore), 
      canAct(true), 
      slowed(false),
      battleMode(BattleMode::CLOSE), 
      hand() {}

// Конструктор перемещения
Player::Player(Player&& other) noexcept
    : Entity(std::move(other)),  // Перемещаем базовую часть Entity
      damage(other.damage), 
      score(other.score), 
      canAct(other.canAct),
      slowed(other.slowed), 
      battleMode(other.battleMode),
      hand(std::move(other.hand)) {}

// Оператор присваивания с перемещением
Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        Entity::operator=(std::move(other));
        damage = other.damage;
        score = other.score;
        canAct = other.canAct;
        slowed = other.slowed;
        battleMode = other.battleMode;
        hand = std::move(other.hand);
    }
    return *this;
}

// Получить текущий урон с учетом режима боя
int Player::getCurrentDamage() const {
    // В реальной реализации здесь будет использоваться конфигурация
    // Пока используем хардкод, позже заменим на конфигурацию
    if (battleMode == BattleMode::CLOSE) {
        return damage + 5;  // Ближний бой: базовый урон + 5
    } else {
        return damage;      // Дальний бой: только базовый урон
    }
}

// Получить радиус атаки в зависимости от режима боя
double Player::getAttackRange() const {
    // В реальной реализации здесь будет использоваться конфигурация
    // Пока используем хардкод, позже заменим на конфигурацию
    if (battleMode == BattleMode::CLOSE) {
        return 1.5;  // Ближний бой: меч - короткая дистанция
    } else {
        return 5.0;  // Дальний бой: лук - большая дистанция
    }
}

// Получить урон - переопределяем метод из Entity
void Player::takeDamage(int damageAmount) {
    Entity::takeDamage(damageAmount);
}

// Переключить режим боя между ближним и дальним
void Player::switchBattleMode() {
    if (battleMode == BattleMode::CLOSE) {
        battleMode = BattleMode::LONG;  // Переключаем на дальний бой
    } else {
        battleMode = BattleMode::CLOSE; // Переключаем на ближний бой
    }
}

// Вызывается в начале хода, если игрок был замедлен в предыдущем ходе
bool Player::applyFrozenEffect() {
    if (slowed) {
        canAct = false;    // Блокируем действие в этот ход
        slowed = false;    // Снимаем замедление после применения эффекта
        return true;       // Эффект применен успешно
    }
    return false;          // Игрок не был замедлен
}

// Начать новый ход игрока - сбрасывает флаг действий
// ⚠️ ВАЖНО: slowed НЕ сбрасывается здесь, он сбрасывается только в applyFrozenEffect
void Player::startNewTurn() {
    canAct = true;
}