#ifndef PLAYER_H
#define PLAYER_H
#include <cstddef>
#include "SpellHand.h"

class Player {
public:
    enum class CombatMode {
        kMelee,
        kRanged
    };

    Player(int max_health,
           int melee_damage,
           int ranged_damage,
           std::size_t hand_size);

    SpellHand &GetSpellHand();

    const SpellHand &GetSpellHand() const;

    int GetRange() const;

    void IncreaseMaxHealth(int delta);

    void IncreaseMeleeDamage(int delta);

    void IncreaseRangedRange(int delta);

    void IncreaseRangedDamage(int delta);

    void RestoreFullHealth();

    int GetHealth() const;

    int GetMaxHealth() const;

    int GetScore() const;

    CombatMode GetCombatMode() const;

    int GetDamage() const;

    bool IsAlive() const;

    void TakeDamage(int amount);

    void AddScore(int amount);

    void SwitchMode();

    bool AddSpellToHand(std::unique_ptr<Spell> spell);

    int GetPendingBuffStacks() const;

    void AddBuffStack(int amount = 1);

    int ConsumeBuffStacks();

    void SetHealth(int hp) { health_ = hp; }
    void SetMaxHealth(int maxhp) { max_health_ = maxhp; }
    void SetScore(int score) { score_ = score; }
    void SetCombatMode(CombatMode mode) { mode_ = mode; }

private:
    int max_health_;
    int health_;
    int melee_damage_;
    int ranged_damage_;
    int score_;
    int ranged_range_;
    CombatMode mode_;
    SpellHand spell_hand_;
    int pending_buff_stacks_ = 0;

    void EnsureInvariants() const;
};

#endif
