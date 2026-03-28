#ifndef HERO_HPP
#define HERO_HPP

#include "entity.hpp"

// режим боя героя(ближний/дальний)
enum class CombatMode { Melee, Ranged };

// класс героя игрока
class Hero : public Entity {
public:
    Hero(int hp, int dmg, Position p): Entity(hp, dmg, p) {}

    // текущий режим боя
    CombatMode getMode() const noexcept { return mode_; }

    // можно ли сейчас ходить (не под эффектом пропуска)
    bool canActThisTurn() const noexcept { return !skip_next_; }

    // переключение режима занимает ход
    bool switchMode() noexcept {
        if (!canActThisTurn()) return false;
        mode_ = (mode_ == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;
        consumed_turn_ = true;
        return true;
    }

    // урон зависит от режима (дальний слабее в 2 раза)
    int setDamage() const noexcept {
        return (mode_ == CombatMode::Melee) ? dmg_ : std::max(1, dmg_ - dmg_ / 2);
    }

    // отметка, что герой вошёл в болото
    void markEnteredSlow() noexcept { slow_flag_ = true; }

    // начало хода: проверка на болото
    void beginTurn() noexcept {
        consumed_turn_ = false;
        if (skip_next_) {
            skipped_this_turn_ = true;
            skip_next_ = false;
        } else {
            skipped_this_turn_ = false;
        }
    }

    // статусы хода
    bool skippedThisTurn() const noexcept { return skipped_this_turn_; }
    bool consumedTurn()  const noexcept { return consumed_turn_; }

    // конец хода: активируем пропуск на следующий, если были на slow
    void endTurn() noexcept {
        if (slow_flag_) { skip_next_ = true; slow_flag_ = false; }
    }

    // помечаем, что действие было выполнено
    void consume() noexcept { consumed_turn_ = true; }

private:
    // внутреннее состояние героя
    CombatMode mode_{CombatMode::Melee};
    bool skip_next_{false};
    bool slow_flag_{false};
    bool consumed_turn_{false};
    bool skipped_this_turn_{false};
};

#endif