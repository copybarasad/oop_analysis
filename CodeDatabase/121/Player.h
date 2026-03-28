#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "BattleMode.h"
#include "SpellHand.h"
#include "GameConfig.h"


// Класс игрока - управляемый персонаж
// Содержит логику игрока: здоровье, урон, режимы боя, заклинания.
// Использует конфигурацию из GameConfig для баланса.


class Player : public Entity {
private:
    int damage;           // Базовый урон игрока
    int score;            // Количество очков игрока
    bool canAct;          // Может ли игрок действовать в этот ход
    bool slowed;          // Замедлен ли игрок
    BattleMode battleMode; // Режим боя (ближний/дальний)
    SpellHand hand;       // Рука с заклинаниями

public:
    // Конструктор по умолчанию - создает игрока с настройками из конфигурации
    Player();
    
    // Конструктор с начальной позицией - создает игрока в указанной позиции
    explicit Player(const Position& startPos);
    
    // Конструктор с конфигурацией - позволяет передать кастомные настройки
    Player(const Position& startPos, const PlayerConfig& config);
    
    // Запрещаем копирование игрока
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    
    // Конструктор перемещения - для эффективного переноса данных
    Player(Player&& other) noexcept;
    
    // Оператор присваивания с перемещением
    Player& operator=(Player&& other) noexcept;
    
    // Деструктор
    ~Player() = default;

    // Геттеры для получения информации об игроке
    
    // Получить базовый урон игрока
    int getDamage() const { return damage; }
    
    // Получить текущий урон с учетом режима боя
    int getCurrentDamage() const;
    
    // Получить радиус атаки в зависимости от режима боя
    double getAttackRange() const;
    
    // Получить количество очков игрока
    int getScore() const { return score; }
    
    // Проверить может ли игрок действовать в этот ход
    bool getCanAct() const { return canAct; }
    
    // Проверить замедлен ли игрок
    bool getSlowed() const { return slowed; }
    
    // Получить текущий режим боя
    BattleMode getBattleMode() const { return battleMode; }
    
    // Получить руку с заклинаниями (для модификации)
    SpellHand& getHand() { return hand; }
    
    // Получить руку с заклинаниями (только для чтения)
    const SpellHand& getHand() const { return hand; }

    // Сеттеры для изменения состояния игрока
    
    // Установить базовый урон
    void setDamage(int newDamage) { damage = newDamage; }
    
    // Установить количество очков
    void setScore(int newScore) { score = newScore; }
    
    // Установить может ли игрок действовать
    void setCanAct(bool canActFlag) { canAct = canActFlag; }
    
    // Установить состояние замедления
    void setSlowed(bool isSlowed) { slowed = isSlowed; }
    
    // Установить режим боя
    void setBattleMode(BattleMode mode) { battleMode = mode; }

    // Получить урон - переопределяем метод из Entity
    void takeDamage(int damageAmount) override;
    
    // Переключить режим боя между ближним и дальним
    void switchBattleMode();
    
    // Применить эффект заморозки (если игрок на замедляющей клетке)
    bool applyFrozenEffect();
    
    // Начать новый ход игрока - сбрасывает флаги действий
    void startNewTurn();
};

#endif