#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "actor.hpp"
#include "hand.hpp"

enum class CombatMode {
    kMelee,
    kRanged
};

class Player : public Actor {
public:
    Player(const std::string& name,
           int hp,
           int melee,
           int ranged,
           Point pos,
           std::size_t hand_capacity = 3,
           int ranged_range = 3);

    const std::string& Name() const noexcept;
    CombatMode Mode() const noexcept;
    bool IsMeleeMode() const noexcept;
    void ToggleMode() noexcept;

    int CurrentDamage() const noexcept;
    int RangedRange() const noexcept;
    int MeleeRange() const noexcept;

    int Score() const noexcept;
    int MaxHP() const noexcept;
    int MeleeDamage() const noexcept;
    int RangedDamage() const noexcept;
    void AddScore(int s) noexcept;
    bool SpendScore(int s) noexcept;

    PlayerHand& Hand() noexcept;
    const PlayerHand& Hand() const noexcept;

    void RestoreHealthToFull() noexcept;
    void IncreaseMaxHealth(int delta);
    void IncreaseMeleeDamage(int delta);
    void IncreaseRangedDamage(int delta);
    void SetScore(int value) noexcept;
    void SetMode(CombatMode mode) noexcept;
    void SetHP(int value);
    void SetMaxHP(int value);
    void SetRangedRange(int value);

private:
    std::string name_;
    int melee_damage_;
    int ranged_damage_;
    CombatMode mode_;
    int score_;
    PlayerHand hand_;
    int ranged_range_;
    int max_hp_;
};

#endif