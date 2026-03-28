#pragma once
#include <vector>
#include <memory>
#include <random>
#include <string>

class Spell;
class Game;

class PlayerHand {
public:
    explicit PlayerHand(std::size_t max_size);
    PlayerHand(const PlayerHand& other);
    PlayerHand& operator=(const PlayerHand& other);
    PlayerHand(PlayerHand&&) noexcept = default;
    PlayerHand& operator=(PlayerHand&&) noexcept = default;

    ~PlayerHand();
    
    std::size_t Size() const;
    std::size_t Capacity() const;

    void PrintHand() const;
    bool AddSpell(std::unique_ptr<Spell> spell);
    void AddRandomSpell(std::mt19937& rng);

    bool CastSpellByIndex(int index, Game& game);

    void AddBoostAll(int value);
    int ConsumeBoostForDirectRange();
    int ConsumeBoostForAreaSize();
    int ConsumeBoostForTrapDamage();
    int ConsumeBoostForSummonCount();

    void IncreaseCapacity(size_t delta);
    void ClearAllSpells();
    void RemoveHalfRandom(std::mt19937& rng);
    const std::vector<std::unique_ptr<Spell>>& Spells() const;

private:
    void ResetAllBoosts_(); 
    std::size_t max_size_;
    std::vector<std::unique_ptr<Spell>> spells_;

    int boost_direct_range_;
    int boost_area_size_;
    int boost_trap_damage_;
    int boost_summon_count_;
};
