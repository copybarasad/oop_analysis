#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "Spell.h"
#include <string>

class Player : public Unit {
public:
    enum class AttackMode { Melee, Ranged };
    explicit Player(int hp = 100, int meleeDamage = 35, int rangedDamage = 30, int rangedRange = 3);

    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getRangedRange() const;
    int getScore() const;

    void addScore(int s);
    bool spendScore(int s);

    AttackMode getAttackMode() const;
    void setAttackMode(AttackMode mode);
    int getAttackDamage() const; // фактический урон в зависимости от режима

    // Усиления от BuffSpell
    void addPendingEnhancement(const SpellEnhancement& enh);
    SpellEnhancement getPendingEnhancement() const;
    void clearPendingEnhancement();

    // Работа с максимальным HP и восстановлением
    int getMaxHP() const;
    void restoreToFullHP();

    // Методы апгрейда (для меню прокачки)
    void addHP(int amount); // добавить maxHP и текущий HP на amount
    void increaseMeleeDamage(int amount);
    void increaseRangedDamage(int amount);
    void increaseRangedRange(int amount);

private:
    int meleeDamage_;
    int rangedDamage_;
    int rangedRange_;
    int score_;
    AttackMode mode_;
    SpellEnhancement pendingEnhancement_;

    int maxHP_; // максимальное здоровье игрока
};

#endif // PLAYER_H
