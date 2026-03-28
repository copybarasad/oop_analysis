#pragma once
#include "Entity.h"
#include "Hand.h"
#include "Enhancement.h"

class GameField;
class Enemy;

enum class AttackMode { Melee, Ranged };

class Player : public Entity {
    Hand hand_;
    Enhancement enh_;
    AttackMode mode_ = AttackMode::Melee;
    int rangedRange_ = 3; // дальность для дальнего удара
    int maxHealth_;
public:
    Player(std::string name, int hp = 30, int dmg = 5, std::size_t handCap = 3)
        : Entity(std::move(name), hp, dmg), hand_(handCap), maxHealth_(hp) {}

    void Update(GameField& field) override;

    Hand& HandRef() noexcept { return hand_; }
    const Hand& HandRef() const noexcept { return hand_; }

    Enhancement& Enh() noexcept { return enh_; }
    const Enhancement& Enh() const noexcept { return enh_; }
    void ConsumeEnhancement() noexcept { enh_.Reset(); }

    // режим атаки
    void SwitchMode();
    AttackMode Mode() const noexcept { return mode_; }
    int AttackRange() const noexcept;
    void PerformAttack(GameField& field);
        // вызываться при переходе на следующий уровень
    void OnLevelUp() {
        // восстановить хп до максимума
        health_ = maxHealth_;
        // случайно удалить половину карт
        hand_.TrimHalfRandom();
    }

    void SetEnhancement(const Enhancement& e) noexcept { enh_ = e; }

};