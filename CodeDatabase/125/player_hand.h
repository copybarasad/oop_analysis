#pragma once
#include "spell.h"
#include <vector>
#include <string>

class Player;
class EnemyManager;
class GameField;

class PlayerHand {
public:
    explicit PlayerHand(size_t max_size = 3);
    ~PlayerHand();

    PlayerHand(const PlayerHand& other);
    PlayerHand& operator=(const PlayerHand& other);

    void Clear();

    const Spell* GetSpell(size_t index) const;
    size_t GetSize() const { return spells_.size(); }
    size_t GetMaxSize() const { return max_size_; }

    bool TryAddSpell(Spell* spell);

    bool CastSpell(size_t index, Player& player, EnemyManager& enemies, GameField& field);

    void AddRandomSpell();
    bool AddSpecificSpell(SpellType type); 

    void RemoveSpell(size_t index);

private:
    std::vector<Spell*> spells_;
    size_t max_size_;

    Spell* CreateInitialSpell() const;     
    Spell* CreateRewardSpell() const;      
    Spell* CreateSpellByType(SpellType type) const;
};